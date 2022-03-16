#include <iostream>
#include <fstream>
// need the `nlohmann-json-git` package for this lol
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

map<string, json> getInput() {
  // Same as getInput, but save variables locally first
  string name, description, url, platform, install, win_startmenu, linux_applications, desktop;

  cout << "Name: "; getline(cin, name);
  cout << "Description: "; getline(cin, description);
  cout << "URL: "; getline(cin, url);
  cout << "Platform: "; getline(cin, platform);

  // should i first ask if this app
  // is for the user's pc or if 
  // they're creating this for a
  // different platform?

  if (platform != "windows" && platform != "linux") {
    cout << "Invalid platform.\n"; 
    return {};
  } else if (platform == "mac") {
    // windows will probably also be
    // unsupported for the time being,
    // as i'm developing this for linux
    cout << "Mac is not supported.\n";
    return {};
  }

  cout << "Would you like to install this as an app? (y/n): "; getline(cin, install);
  if (install == "y") install = "true";
  else install = "false";

  // optional questions
  if (install == "true") {
    if (platform == "windows") {
      cout << "Would you like to add this to the start menu? (y/n): "; getline(cin, win_startmenu);
    } else if (platform == "linux") {
      cout << "Would you like to add this to /usr/share/applications? (y/n): "; getline(cin, linux_applications);
    } 
    if (win_startmenu == "y") {
      win_startmenu = "true";
    } else {
      win_startmenu = "false";
    }
    cout << "Would you like to add this to the desktop? (y/n): "; getline(cin, desktop);
    if (desktop == "y") {
      desktop = "true";
    } else {
      desktop = "false";
    }
  }

  // now let's return this data as an object
  json j;

  // populate :)
  j["name"] = name;
  j["description"] = description;
  j["url"] = url;
  j["platform"] = platform;
  j["install"] = install;
  if (install == "true") {
    j["start"] = win_startmenu;
    j["desktop"] = desktop;
  }

  return j;
}

void writeJSON(map<string, json> data) {
  // create our json file :)
  ofstream dest;
  dest.open("data.json");

  // make our json pretty while we're at it
  dest << setw(4) << data << endl;
  dest.close();
  return;
}

int main() {
  json data = getInput();

  if (data.empty()) {
    cout << "Error: Invalid values.\n";
    // i'll bug the user to re-enter the data until they get it right later lol
    return 1;
  }

  writeJSON(data);

  // we'll also write to the user's 
  // appdata / .local folder later,
  // keeping track of all of their 
  // electrify apps for easy access
  // and management, unless they're
  // exporting for a different platform

  // if (data.install == "true") {
  //   track the app 
  // }

  // just an idea, might change later
  // localFolder/electrify/apps/appName/data.json
  //
  // {
  //   name: "",
  //   description: "",
  //   path: "",
  //   installed_at: "",
  // }

  // for testing the newly created data:
  system("npm run start");

  // here we'll create shortcuts,
  // move em in the right places,
  // and bam! we're done! :D

  return 0;
}
