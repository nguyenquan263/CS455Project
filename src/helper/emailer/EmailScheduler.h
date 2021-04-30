// Written by Micah Edmonds
//
//  TODO: May need to adjust functionality for accepting a start date 
//        for a scheduling job as we determine how that will need to be done
//
//  TODO: If we are going to be generating reports ourselves, then this class
//        will need to accept arguments and include that code to allow it generate
//        the reports at each scheduled time
//
// ***NOTE
//
//      This class uses the libcurl and libcron libraries to schedule an email report
//      to be sent. A user of this class need only instantiate it (no need for 
//      constructor) and then call one of the reporting methods from that instance.
//
//      Libcron doesn't require a linked library, so I've just copied all of the dependent files for libcron 
//      into this emailer directory, and grabbed them with local includes, so no need 
//      to check or install libcron on the target machine. We can change this if need
//      be, but I'm not sure it would be worth the effort.
//


#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

#include "Mailer.h"

#include "libcron/Cron.h"
#include "libcron/Task.h"
#include "libcron/src/CronClock.cpp"
#include "libcron/src/CronData.cpp"
#include "libcron/src/CronRandomization.cpp"
#include "libcron/src/CronSchedule.cpp"
#include "libcron/src/Task.cpp"

using namespace std;

// EmailScheduler class
// 
// This class handles scheduling emailing jobs, whether
// they are one time reports or recurring reports
class EmailScheduler
{
    using TaskFunction = function<void(const libcron::TaskInformation&)>;

    private:
        int recurringPeriod;
        string cronTimeString;
        string emailDestination;
        string reportFilePath;

        thread taskThread;
        TaskFunction taskToRun;

        // method to run in thread so the rest of the software 
        // can run without monitoring the job
        void ReportHelper(libcron::Cron<libcron::LocalClock, libcron::NullLock> cron)
        {  
            // create the email command
            string reportFilePath = this->reportFilePath;
            string emailDestination = this->emailDestination;
            int recurringPeriod = this->recurringPeriod;
            bool taskComplete = false;

            taskToRun = [reportFilePath, emailDestination, recurringPeriod, &taskComplete]
                        (auto& i) mutable
            {
                // send the email
                Mailer mailer(emailDestination, reportFilePath);

                // if task was a one time job, clear the cron schedule
                if (recurringPeriod == 0)
                {
                    taskComplete = true;
                }
            };

            cron.add_schedule("Email Report", cronTimeString, taskToRun);

            while (!taskComplete)
            {
                cron.tick();
            }

            cron.clear_schedules();
        }

        // helper function used by both report functions.
        // creates a cron object and starts a thread of the function that
        // will schedule the cron job, then detaches it
        void RunEmailReport()
        {
            libcron::Cron<libcron::LocalClock, libcron::NullLock> cron;

            taskThread = thread(&EmailScheduler::ReportHelper, this, cron);
            taskThread.detach();
        }

        // method that converts the given string, expected to be
        // formatted like the output from to_simple_string(ptime t):
        //
        //                  2021-Apr-26 17:57:12
        //
        // and converts it to a valid cron time string
        string ConvertToCronTime(string timeString)
        {
            // split the given string into tokens based on expected delimiters
            vector<string> tokens;
            auto cTimeString = timeString.c_str();

            do
            {
                const char *begin = cTimeString;

                while(*cTimeString != ' ' && *cTimeString != ':' && *cTimeString != '-' && *cTimeString)
                {
                    cTimeString++;
                }

                tokens.push_back(string(begin, cTimeString));
            } while (0 != *cTimeString++);

            // adjust the day value depending on if the job is recurring
            string dayAndStep;

            if (recurringPeriod != 0)
                dayAndStep = tokens.at(2) + "/" + to_string(recurringPeriod);
            else   
                dayAndStep = tokens.at(2);

            // build the cron string based on these values
            string cronString = "* " +                                      // second
                                tokens.at(4) + " " +                        // minute
                                tokens.at(3) + " " +                        // hour
                                dayAndStep + " " +                        // day of month
                                AbrvToMonthNumber(tokens.at(1)) + " " +     // month
                                "?";                                        // day of week

            return cronString;
        }

        // helper function to convert month abreviation to number
        string AbrvToMonthNumber(string abreviation)
        {
            string monthNumber;

            if (abreviation == "Jan")
                monthNumber = "1";
            else if (abreviation == "Feb")
                monthNumber = "2";
            else if (abreviation == "Mar")
                monthNumber = "3";
            else if (abreviation == "Apr")
                monthNumber = "4";
            else if (abreviation == "May")
                monthNumber = "5";
            else if (abreviation == "Jun")
                monthNumber = "6";
            else if (abreviation == "Jul")
                monthNumber = "7";
            else if (abreviation == "Aug")
                monthNumber = "8";
            else if (abreviation == "Sep")
                monthNumber = "9";
            else if (abreviation == "Oct")
                monthNumber = "10";
            else if (abreviation == "Nov")
                monthNumber = "11";
            else
                monthNumber = "12";

            return monthNumber;
        }

        // helper function to make scheduling different reports more streamlined
        void ScheduleReport(string emailDestination, string reportFilePath, string whenToRun)
        {
            this->emailDestination = emailDestination;
            this->reportFilePath = reportFilePath;

            this->cronTimeString = ConvertToCronTime(whenToRun);

            RunEmailReport();
        }

    public:

        // schedule a report to be sent one time
        // 
        // string emailDestination - email address to send the email to
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run, expects the string
        //                    is formatted like the output from to_simple_string(ptime t):
        //
        //                                            2021-Apr-26 17:57:12
        //
        //Cron time string:
        //      -----------------  second (0 - 59)
        //      | ------------------  minute (0 - 59)
        //      | | ------------------  hour (0 - 23)
        //      | | | ------------------  day of month (1 - 31)
        //      | | | | ------------------  month (1 - 12)
        //      | | | | | ------------------- day of week (0 - 6) (Sunday - Saturday)
        //      | | | | | |
        //      * * * * * *
        //
        void ScheduleOneTimeReport(string emailDestination, string reportFilePath, string whenToRun)
        {
            this->recurringPeriod = 0;
            ScheduleReport(emailDestination, reportFilePath, whenToRun);
        }

        // schedule a report to be sent daily
        // 
        // string emailDestination - email address to send the email to
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run, expects the string
        //                    is formatted like the output from to_simple_string(ptime t):
        //
        //                                            2021-Apr-26 17:57:12
        //
        // **See function documentation for ScheduleOneTimeReport for cron time diagram
        void ScheduleDailyReport(string emailDestination, string reportFilePath, string whenToRun)
        {
            this->recurringPeriod = 1;
            ScheduleReport(emailDestination, reportFilePath, whenToRun);
        }

        // schedule a report to be sent weekly
        // 
        // string emailDestination - email address to send the email to
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run, expects the string
        //                    is formatted like the output from to_simple_string(ptime t):
        //
        //                                            2021-Apr-26 17:57:12
        //
        // **See function documentation for ScheduleOneTimeReport for cron time diagram
        void ScheduleWeeklyReport(string emailDestination, string reportFilePath, string whenToRun)
        {
            this->recurringPeriod = 7;
            ScheduleReport(emailDestination, reportFilePath, whenToRun);
        }
};