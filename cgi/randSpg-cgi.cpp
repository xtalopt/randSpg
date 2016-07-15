/***************** start of the getcgivars() module **********************/

/*************************************************************************/
/**                                                                     **/
/**     getcgivars.c-- routine to read CGI input variables into an      **/
/**         array of strings.                                           **/
/**                                                                     **/
/**     Written in 1996 by James Marshall, james@jmarshall.com, except  **/
/**     that the x2c() and unescape_url() routines were lifted directly **/
/**     from NCSA's sample program util.c, packaged with their HTTPD.   **/
/**                                                                     **/
/**     For the latest, see http://www.jmarshall.com/easy/cgi/ .        **/
/**                                                                     **/
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** Convert a two-char hex string into the char it represents. **/
char x2c(char *what) {
  register char digit;

  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
  return(digit);
}

/** Reduce any %xx escape sequences to the characters they represent. **/
void unescape_url(char *url) {
  register int i,j;

  for(i=0,j=0; url[j]; ++i,++j) {
    if((url[i] = url[j]) == '%') {
      url[i] = x2c(&url[j+1]) ;
      j+= 2 ;
    }
  }
  url[i] = '\0' ;
}


/** Read the CGI input and place all name/val pairs into list.        **/
/** Returns list containing name1, value1, name2, value2, ... , NULL  **/
char **getcgivars() {
  register int i ;
  char *request_method ;
  int content_length;
  char *cgiinput ;
  char **cgivars ;
  char **pairlist ;
  int paircount ;
  char *nvpair ;
  char *eqpos ;

  /** Depending on the request method, read all CGI input into cgiinput. **/
  request_method= getenv("REQUEST_METHOD") ;

  if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD") ) {
    /* Some servers apparently don't provide QUERY_STRING if it's empty, */
    /*   so avoid strdup()'ing a NULL pointer here.                      */
    char *qs ;
    qs= getenv("QUERY_STRING") ;
    cgiinput= strdup(qs  ? qs  : "") ;
  }
  else if (!strcmp(request_method, "POST")) {
    /* strcasecmp() is not supported in Windows-- use strcmpi() instead */
    if ( strcasecmp(getenv("CONTENT_TYPE"), "application/x-www-form-urlencoded")) {
      printf("Content-Type: text/plain\n\n") ;
      printf("getcgivars(): Unsupported Content-Type.\n") ;
      exit(1) ;
    }
    if ( !(content_length = atoi(getenv("CONTENT_LENGTH"))) ) {
      printf("Content-Type: text/plain\n\n") ;
      printf("getcgivars(): No Content-Length was sent with the POST request.\n") ;
      exit(1) ;
    }
    if ( !(cgiinput= (char *) malloc(content_length+1)) ) {
      printf("Content-Type: text/plain\n\n") ;
      printf("getcgivars(): Couldn't malloc for cgiinput.\n") ;
      exit(1) ;
    }
    if (!fread(cgiinput, content_length, 1, stdin)) {
      printf("Content-Type: text/plain\n\n") ;
      printf("getcgivars(): Couldn't read CGI input from STDIN.\n") ;
      exit(1) ;
    }
    cgiinput[content_length]='\0' ;
  }
  else {
    printf("Content-Type: text/plain\n\n") ;
    printf("getcgivars(): Unsupported REQUEST_METHOD.\n") ;
    exit(1) ;
  }

  /** Change all plusses back to spaces. **/
  for (i=0; cgiinput[i]; i++) if (cgiinput[i] == '+') cgiinput[i] = ' ' ;

  /** First, split on "&" and ";" to extract the name-value pairs into **/
  /**   pairlist.                                                      **/
  pairlist= (char **) malloc(256*sizeof(char **)) ;
  paircount= 0 ;
  nvpair= strtok(cgiinput, "&;") ;
  while (nvpair) {
    pairlist[paircount++]= strdup(nvpair) ;
    if (!(paircount%256))
      pairlist= (char **) realloc(pairlist,(paircount+256)*sizeof(char **)) ;
    nvpair= strtok(NULL, "&;") ;
  }
  pairlist[paircount]= 0 ;    /* terminate the list with NULL */

  /** Then, from the list of pairs, extract the names and values. **/
  cgivars= (char **) malloc((paircount*2+1)*sizeof(char **)) ;
  for (i= 0; i<paircount; i++) {
    if (eqpos=strchr(pairlist[i], '=')) {
      *eqpos= '\0' ;
      unescape_url(cgivars[i*2+1]= strdup(eqpos+1)) ;
    } else {
      unescape_url(cgivars[i*2+1]= strdup("")) ;
    }
    unescape_url(cgivars[i*2]= strdup(pairlist[i])) ;
  }
  cgivars[paircount*2]= 0 ;   /* terminate the list with NULL */

  /** Free anything that needs to be freed. **/
  free(cgiinput) ;
  for (i=0; pairlist[i]; i++) free(pairlist[i]) ;
  free(pairlist) ;

  /** Return the list of name-value strings. **/
  return cgivars ;

}

/***************** end of the getcgivars() module ********************/

// For timings
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

#include "elemInfo.h"
#include "randSpg.h"
#include "randSpgOptions.h"
#include "utilityFunctions.h"

using namespace std;

int invalidInput(std::string errorMsg)
{
  printf("Content-type: text/html\n\n");
  printf("<html>\n");
  printf("<head><title>RandSpg Results</title></head>\n");
  printf("<body>\n");
  printf("<h1>Invalid input</h1>\n");
  printf("%s", errorMsg.c_str());
  printf("<br><br><br>");
  printf("</body>\n");
  printf("</html>\n");
  exit(-1);
}

int main() {

  // Let's time the setup!
  auto start_setupWall = std::chrono::high_resolution_clock::now();

  // Remove the old log file before doing anything else...
  remove(e_logfilename.c_str());

  // Obtain the cgi variables
  char** cgivars = getcgivars();
  char* in = NULL;

  for (int i=0; cgivars[i]; i+= 2) {
    char *key = cgivars[i];
    char *val = cgivars[i+1];
    // This should be the only cgi variable
    if (strcmp(key, "randSpgInput") == 0) in = val;
  }

  // Add newline characters to the ends of these lines so we can read the
  // input correctly
  string stdstr(in);
  istringstream lines(stdstr);
  string inputStr, line;
  while (getline(lines, line)) inputStr += (line + "\n");

  RandSpgOptions options = RandSpgOptions::readOptionsFromCharArray(inputStr.c_str());

  if (!options.optionsAreValid())
    return invalidInput("Some options are invalid. Please check your input.");

  std::stringstream ss;
  ss << "Content-type: text/html\n\n"
     << "<html>\n"
     << "<head><title>RandSpg Results</title></head>\n"
     << "<body>\n";

  // Let's print the spg options to the start of this unless verbosity is 'n'
  if (options.getVerbosity() != 'n') {
    ss << "<h3>Options:</h3>\n";
    ss << useHTMLReturns(options.getOptionsString()) << "<br>";
  }

  ss << "<h3>POSCARs:</h3>\n";

  std::string comp = options.getComposition();
  std::vector<uint> atoms;
  string errMsg;
  errMsg += "'"; errMsg += (comp + "'");
  if (!ElemInfo::readComposition(trim(comp), atoms))
    return invalidInput(string("Error: composition, ") + comp
                        + "is an invalid composition");

  // We don't want to use a log file here
  e_verbosity = 'n';

  // Set up lattice mins and maxes
  latticeStruct mins  = options.getLatticeMins();
  latticeStruct maxes = options.getLatticeMaxes();

  // Create the input
  randSpgInput input(1, atoms, mins, maxes);

  // Add various other input options
  input.IADScalingFactor = options.getScalingFactor();
  input.minRadius = options.getMinRadii();
  input.manualAtomicRadii = options.getRadiusVector();
  input.minVolume = options.getMinVolume();
  input.maxVolume = options.getMaxVolume();
  input.forcedWyckAssignments = options.getForcedWyckAssignments();
  input.verbosity = options.getVerbosity();
  input.maxAttempts = options.getMaxAttempts();
  input.forceMostGeneralWyckPos = options.forceMostGeneralWyckPos();

  // Set up various other options
  vector<uint> spacegroups = options.getSpacegroups();
  int numOfEach = options.getNumOfEachSpgToGenerate();
  string outDir = options.getOutputDir();

  size_t numSucceeds = 0;
  size_t numAttempts = spacegroups.size() * numOfEach;

  // Perform final checks
  // For the online version, we will only allow up to 250 structures to be
  // generated so that we don't overload the server
  size_t maxNumStructures = 250;
  if (numAttempts > maxNumStructures) {
    stringstream error;
    error << "Error: for the html version of this program, we only allow up to " << maxNumStructures
          << " to be generated so we do not overload the server.<br> You are "
          << "requesting " << numAttempts << " structures. Please reduce this "
          << "amount or compile and use the executable version of the "
          << "program <a href=\"http://www.github.com/psavery/randSpg\">here</a>.<br>\n";
    invalidInput(error.str());
  }

  size_t maxNumAtoms = 50;
  if (atoms.size() > maxNumAtoms) {
    stringstream error;
    error << "Error: for the html version of this program, we only allow up to " << maxNumAtoms
          << " atoms per crystal so that we do not overload the server.<br> Your composition "
          << "has " << atoms.size() << " atoms. Please reduce this "
          << "amount or compile and use the executable version of the "
          << "program <a href=\"http://www.github.com/psavery/randSpg\">here</a>.<br>\n";
    invalidInput(error.str());
  }

  size_t maxAttempts = 50;
  if (input.maxAttempts > maxAttempts) {
    stringstream error;
    error << "Error: for the html version of this program, we only allow up to " << maxAttempts
          << " max attempts per crystal so that we do not overload the server.<br> You have requested "
          << input.maxAttempts << " max attempts. Please reduce this "
          << "amount or compile and use the executable version of the "
          << "program <a href=\"http://www.github.com/psavery/randSpg\">here</a>.<br>\n"
          << "Note: if you do not specify the maxAttempts, the default is 100. So you will need to "
          << "specify it to 50 or less<br>\n";
    invalidInput(error.str());
  }

  auto setupWallTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start_setupWall).count() * 1e-9;

  // Let's time it!
  auto start_loopWall = std::chrono::high_resolution_clock::now();

  // Loop through the ones we are going to create
  for (size_t i = 0; i < spacegroups.size(); i++) {
    uint spg = spacegroups.at(i);
    // Change the input spg to have the right spacegroup
    input.spg = spg;
    for (size_t j = 0; j < numOfEach; j++) {
      Crystal c = RandSpg::randSpgCrystal(input);

      // The volume is set to zero if the job failed.
      if (c.getVolume() == 0) {
        ss << comp + " -- randSpg with spg of: " + to_string(spg)
           << "<br>\nFailed to generate crystal. Either it is impossible "
           << "to generate this crystal with these settings or it failed to "
           << "generate a crystal after " << input.maxAttempts << " attempts.<br>"
           << "Use the <a href=\"http://www.github.com/psavery/randSpg\">executable version</a> "
           << "of the program for more detailed info"
           << "<br><br>\n";
        continue;
      }

      // If it created a crystal successfully, add this time to the total time
      numSucceeds++;

      // Let's print it to the HTML script!
      string title = comp + " -- randSpg with spg of: " + to_string(spg);
      ss << useHTMLReturns(c.getPOSCARString(title));

      ss << "<br><br>\n";
    }
  }

  auto loopWallTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start_loopWall).count() * 1e-9;

  ss << "<h3>Note:</h3>"
     << "Please download and use the executable version if you wish to see log "
     << "info like the Wyckoff positions each atom was assigned to.<br><br>\n"
     << "Number of structures attempted: " << numAttempts << "<br>\n"
     << "Number of succeeds: " << numSucceeds << "<br>\n"
     << "Setup time (in seconds): " << setupWallTime << "<br>\n"
     << "Structure generation time (in seconds): " << loopWallTime << "<br>\n"
     << "Average time per structure attempted (in seconds): "
     << loopWallTime / ((double)numAttempts) << "<br><br>\n"
     << "Times this webpage has been used since March 17th 2016: "
     << "<br><embed src=\"randSpgCounter.cgi\" /><br>\n"
     << "</body>\n"
     << "</html>\n";

  // Let's print it!
  printf("%s", ss.str().c_str());

  /** Free anything that needs to be freed **/
  for (int i=0; cgivars[i]; i++) free(cgivars[i]) ;
  free(cgivars);

  exit(0);
}

std::string debug;

void Debug(const char *str, const double d)
{
  char buffer[128];
  snprintf(buffer, 32, "%s %f\n", str, d);
  debug += buffer;
}
void Debug(const std::string &str, const double d) {Debug(str.c_str(), d);}

