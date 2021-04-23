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
//      I just included all source files so that the code would function. We can fix
//      this at some point as well.


#include <iostream>
#include <fstream>
#include <thread>

#include <libcron/Cron.h>
#include <libcron/Task.h>
#include <libcron/src/CronClock.cpp>
#include <libcron/src/CronData.cpp>
#include <libcron/src/CronRandomization.cpp>
#include <libcron/src/CronSchedule.cpp>
#include <libcron/src/Task.cpp>

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

        bool oneTimeJob;
        string whenToRun;
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
            bool oneTimeJob = this->oneTimeJob;
            bool taskComplete = false;

            taskToRun = [emailSubject, reportFilePath, emailDestination, tempPath, content, oneTimeJob, &taskComplete]
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
                if (oneTimeJob)
                {
                    taskComplete = true;
                }

                // get rid of the temp file
                system(("rm " + tempPath).c_str());
            };

            cron.add_schedule("Email Report", whenToRun, taskToRun);

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

    public:

        // schedule a report to be sent one time
        // 
        // string emailDestination - email address to send the email to
        // string emailSubject - text for the subject line of the email
        // string emailContent - text to be inserted in the email body
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run in cron terms
        //
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
            this->oneTimeJob = true;
            this->emailDestination = emailDestination;
            this->emailSubject = emailSubject;
            this->emailContent = emailContent;
            this->reportFilePath = reportFilePath;
            this->whenToRun = whenToRun;

            RunEmailReport();
        }

        // schedule a report to be sent, and then repeat
        // 
        // string emailDestination - email address to send the email to
        // string emailSubject - text for the subject line of the email
        // string emailContent - text to be inserted in the email body
        // string reportFilePath - file path to the report to be attached to this email
        // string whenToRun - string to indicate when the job should be run in cron terms
        //
        // **See function documentation for ScheduleOneTimeReport for whenToRun diagram
        void ScheduleRecurringReport(string emailDestination, string emailSubject, string emailContent, string reportFilePath, string whenToRun)
        {
            this->oneTimeJob = false;
            this->emailDestination = emailDestination;
            this->emailSubject = emailSubject;
            this->emailContent = emailContent;
            this->reportFilePath = reportFilePath;
            this->whenToRun = whenToRun;

            RunEmailReport();
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