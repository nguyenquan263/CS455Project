// Written by Micah Edmonds
//
// ***NOTE
//
//      Right now, this class is dependent on the machine it runs on to have the
//      mail utility 'mutt' installed, as it calls it using the system() function.
//      This class will need to be updated if we change how we approach mailing, or
//      if Dr. Jerkins does not want to install mutt on the cs.csis.work server.
//
//      This class is also still in progress. I still need to make changes to allow the
//      class to accept date information and convert it to the appropriate libcron
//      time schedule.
//
//      Once we have started setting up the code that needs to send direct emails or
//      schedule regular emails (like for sending reports), this class can be 
//      instantiated from that code to get the job done in a few lines.
//
//      Lastly, this code depends on usage of the libcron library. I'm clueless when
//      it comes to cmake, make, or really any kind of c++ compiling and linking, so
//      I've just copied all of the dependent files into this emailer directory, and
//      grabbed them with local includes. We can change this if need be, but I'm not
//      sure it would be worth the effort.


#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

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
// they are one time reports, recurring reports, or just
// instant emails
class EmailScheduler
{
    using TaskFunction = function<void(const libcron::TaskInformation&)>;

    private:
        string tempPath = "./temp.txt";
        string defaultReportContent = "See attachment for the requested report.";

        int recurringPeriod;
        string cronTimeString;
        string emailDestination;

        string emailSubject;
        string emailContent;
        string reportFilePath;

        thread taskThread;
        TaskFunction taskToRun;

        // method to run in thread so the rest of the software 
        // can run without monitoring the job
        void ReportHelper(libcron::Cron<libcron::LocalClock, libcron::NullLock> cron)
        {  
            // create the email command 
            string emailSubject = this->emailSubject;
            string reportFilePath = this->reportFilePath;
            string emailDestination = this->emailDestination;
            string tempPath = this->tempPath;
            string content = this->defaultReportContent;
            int recurringPeriod = this->recurringPeriod;
            bool taskComplete = false;

            taskToRun = [emailSubject, reportFilePath, emailDestination, tempPath, content, recurringPeriod, &taskComplete]
                        (auto& i) mutable
            {
                // due to a limitation with mutt, we must write the content of the
                // email to an output file
                ofstream outfile;
                outfile.open(tempPath);
                outfile << content << endl;
                outfile.close();

                // add the file that contains content to the command
                string command = "mutt -s \"" + emailSubject + "\" -a " + reportFilePath + " " + emailDestination + " < " + tempPath;

                // make a system call using the mailing utility
                //system(command.c_str());
                system(("echo \"" + command + "\" >> /home/micah/clientTest.txt").c_str());

                // if task was a one time job, clear the cron schedule
                if (recurringPeriod == 0)
                {
                    taskComplete = true;
                }

                // get rid of the temp file
                system(("rm " + tempPath).c_str());
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

            // build the cron string based on these tokens
            string dayAndStep;

            if (recurringPeriod != 0)
                dayAndStep = tokens.at(2) + "/" + to_string(recurringPeriod);
            else   
                dayAndStep = tokens.at(2);


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
        void ScheduleReport(string emailDestination, string emailSubject, string emailContent, string reportFilePath, string whenToRun)
        {
            this->emailDestination = emailDestination;
            this->emailSubject = emailSubject;
            this->emailContent = emailContent;
            this->reportFilePath = reportFilePath;

            // convert time string to fit with cron job timing
            this->cronTimeString = ConvertToCronTime(whenToRun);

            RunEmailReport();
        }

    public:

        // schedule a report to be sent one time
        // 
        // string emailDestination - email address to send the email to
        // string emailSubject - text for the subject line of the email
        // string emailContent - text to be inserted in the email body
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
        void ScheduleOneTimeReport(string emailDestination, string emailSubject, string emailContent, string reportFilePath, string whenToRun)
        {
            this->recurringPeriod = 0;
            ScheduleReport(emailDestination, emailSubject, emailContent, reportFilePath, whenToRun);
        }

        // schedule a report to be sent daily
        // 
        // string emailDestination - email address to send the email to
        // string emailSubject - text for the subject line of the email
        // string emailContent - text to be inserted in the email body
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run, expects the string
        //                    is formatted like the output from to_simple_string(ptime t):
        //
        //                                            2021-Apr-26 17:57:12
        //
        // **See function documentation for ScheduleOneTimeReport for cron time diagram
        void ScheduleDailyReport(string emailDestination, string emailSubject, string emailContent, string reportFilePath, string whenToRun)
        {
            this->recurringPeriod = 1;
            ScheduleReport(emailDestination, emailSubject, emailContent, reportFilePath, whenToRun);
        }

        // schedule a report to be sent weekly
        // 
        // string emailDestination - email address to send the email to
        // string emailSubject - text for the subject line of the email
        // string emailContent - text to be inserted in the email body
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run, expects the string
        //                    is formatted like the output from to_simple_string(ptime t):
        //
        //                                            2021-Apr-26 17:57:12
        //
        // **See function documentation for ScheduleOneTimeReport for cron time diagram
        void ScheduleWeeklyReport(string emailDestination, string emailSubject, string emailContent, string reportFilePath, string whenToRun)
        {
            this->recurringPeriod = 7;
            ScheduleReport(emailDestination, emailSubject, emailContent, reportFilePath, whenToRun);
        }

        // send an email to this destination right away
        // 
        // string emailDestination - email address to send the email to
        // string emailSubject - text for the subject line of the email
        // string emailContent - text to be inserted in the email body
        // string attachmentFilePath (optional) - file path to an attachment
        //
        void SendEmail(string emailDestination, string emailSubject, string emailContent, string attachmentFilePath="")
        {
            // due to the limitations of mutt, we must write the content to a file
            // and then include it in the email command
            ofstream outfile;
            outfile.open(tempPath);
            outfile << emailContent;
            outfile.close();

            // build email command based on whether or not attachment was included
            string command;
            
            if (attachmentFilePath=="")
            {
                command = "mutt -s \"" + emailSubject + "\" " + emailDestination + " < " + tempPath;
            }
            else
            {
                command = "mutt -s \"" + emailSubject + "\" -a " + attachmentFilePath + " " + emailDestination + " < " + tempPath;
            }

            // send the email
            //system(command.c_str());
            cout << command << endl;

            // delete the email file
            system(("rm " + tempPath).c_str());
        }
};