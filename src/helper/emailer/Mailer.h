#include <cstdio>
#include <ctime>
#include <string>
#include <curl/curl.h>

using namespace std;

//
// Mailer.h defines a class that will handle sending 
// emails with base64 encoded attachements.
//
// Right now, emails are sent through google's smtp server, using
// the credentials of the "cs455Student@gmail.com" account. If this
// needs to change at any point, just edit the member variables smtpServer,
// from, username, and password as appropriate.
//

class Mailer
{
	private:
		// member variables. There is a distinction between to and from with their
		// header forms to deal with formatting for different sections of the email
		string date;
		string to;
		string toHeader;

		string from = "cs455Student@gmail.com";
		string fromHeader; // this is built in the constructor using the from string
		string username = "cs455Student@gmail.com";
		string password = "cs455IsHard!";
		string smtpServer = "smtps://smtp.gmail.com";
		
		string messageID; 
		string subject = "Subject: Report Generated";
		string attachmentPath;
		
		// This function assumes the argument strings have been built properly,
		// and handles all of the curl mailing process
		int SendEmail()
		{
			// convert argument strings to c_str's
			const char * cDate = date.c_str();
			const char * cTo = toHeader.c_str();
			const char * cFrom = fromHeader.c_str();
			const char * cMessageID = messageID.c_str();
			const char * cSubject = subject.c_str();

			// build the headers text array using c_str versions of argument strings
			// for compatibility with curl's functions
			const char *headers_text[] = {
				cDate,
				cTo,
				cFrom,
				cMessageID,
				cSubject,
				NULL
			};
			 
			// build the text that will be included in the email's content
			const char inline_text[] =
				"See attached file for your requested report.\r\n";

			CURL *curl;
			CURLcode res = CURLE_OK;
		 
			curl = curl_easy_init();
		  
			if(curl) 
			{
				struct curl_slist *headers = NULL;
				struct curl_slist *recipients = NULL;
				struct curl_slist *slist = NULL;
				curl_mime *mime;
				curl_mime *alt;
				curl_mimepart *part;
				const char **cpp;

				/* This code sets up usage with the smtp server */
				curl_easy_setopt(curl, CURLOPT_URL, smtpServer.c_str());
				curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
				curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

				/* Set up sender and recipients */
				curl_easy_setopt(curl, CURLOPT_MAIL_FROM, username.c_str());

				recipients = curl_slist_append(recipients, to.c_str());
				curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

				/* Build and set the message header list. */
				for(cpp = headers_text; *cpp; cpp++)
					headers = curl_slist_append(headers, *cpp);

				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

				/* Build the mime message. */
				mime = curl_mime_init(curl);

				/* The inline part is an alternative proposing the html and the text
				versions of the e-mail. */
				alt = curl_mime_init(curl);

				/* Text message. */
				part = curl_mime_addpart(alt);
				curl_mime_data(part, inline_text, CURL_ZERO_TERMINATED);

				/* Create the inline part. */
				part = curl_mime_addpart(mime);
				curl_mime_subparts(part, alt);
				curl_mime_type(part, "multipart/alternative");
				slist = curl_slist_append(NULL, "Content-Disposition: inline");
				curl_mime_headers(part, slist, 1);

				/* Encode and add an attachment. */
				part = curl_mime_addpart(mime);
				curl_mime_filedata(part, attachmentPath.c_str());
				curl_mime_encoder(part, "base64");
				curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

				// Trigger verbose mode for debugging
				//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

				/* Send the message */
				res = curl_easy_perform(curl);

				/* Check for errors */
				if(res != CURLE_OK)
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

				/* Free lists. */
				curl_slist_free_all(recipients);
				curl_slist_free_all(headers);

				curl_easy_cleanup(curl);

				/* Free multipart message. */
				curl_mime_free(mime);
			}

			return (int)res;
		}
		
	public:
	
		// constructor is the only needed function. Takes in destination address and attachement path,
		// and handles the rest internally
		Mailer(string destinationAddress, string attachmentPath)
		{
			// build the argument strings needed for the mail header
			// Skip the 'from' and 'subject' strings, since it is fine as default for now
			date = "Date: " + to_string(time(NULL));
			fromHeader = "From: " + from;
			to = destinationAddress;
			toHeader = "To: " + destinationAddress;

			// create unique message ID using current time
			messageID = "Message-ID: <" + to_string(time(NULL)) + "@cs455TutorServer>";
			
			// this->attachmentPath = attachmentPath;
			
			// Preliminary work is done, so send the email now
			SendEmail();
		}
};



