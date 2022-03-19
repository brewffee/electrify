#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

string getOS() {
  #if defined(_WIN32) || defined(_WIN64)
    return "windows";
  #elif defined(__APPLE__) || defined(__MACH__)
    return "mac";
  #elif __linux__
    return "linux";
  #endif
}

map<string, json> getInput() {
  string name, description, url, platform, install, /*win_startmenu,*/ linux_applications, desktop;

  // warning for unsupported platforms
  if (getOS() == "windows" || getOS() == "mac") {
    cout << "\e[1mWarning: Creating applications for " << getOS() << " is not supported yet.\n";
    cout << "\e[1mIf you would like to recieve support for your platform sooner, please help us by contributing to the project on GitHub.\n\n\e[0m"; 
  }

  // add handling for required fields and default values
  cout << "What is the name of the application? (default: \"Electrify-App\")\n > "; getline(cin, name);
  cout << "What is the description of the application? (default: blank)\n > "; getline(cin, description);
  cout << "Enter the desired URL (required)\n > "; getline(cin, url);
  cout << "What is the target platform of the application? (default: " << getOS() << ")\n > "; getline(cin, platform);

  if (platform == "mac" || platform == "windows") {
    cout << "The selected platform '" << platform << "' is not supported.\n";
    return {};
  } else if (platform != "linux") {
    cout << "Invalid platform.\n"; 
    return {};
  } 

  // installation only for matching platforms
  if (platform == getOS()) {
    cout << "Would you like to install this as an app? (y/n)\n > "; getline(cin, install);
    if (install == "y") install = "true";
    else install = "false";

    // optional questions
    if (install == "true") {
      /*if (platform == "windows") {
        cout << "Would you like to add this to the start menu? (y/n)\n > "; getline(cin, win_startmenu);
      } else*/ if (platform == "linux") {
        cout << "Would you like to add this to /usr/share/applications? (y/n)\n > "; getline(cin, linux_applications);
      } 
      /*if (win_startmenu == "y") {
        win_startmenu = "true";
      } else {
        win_startmenu = "false";
      }*/
      cout << "Would you like to add this to the desktop? (y/n)\n > "; getline(cin, desktop);
      if (desktop == "y") {
        desktop = "true";
      } else {
        desktop = "false";
      }
    }
  } else {
    install = "false";
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
    /*j["start"] = win_startmenu;*/
    j["appmenu"] = linux_applications;
    j["desktop"] = desktop;
  }

  return j;
}

void writeJSON(map<string, json> data, string name) {
  // create our json file :)
  ofstream dest;
  dest.open(name + ".json");

  // make our json pretty while we're at it
  dest << setw(4) << data << endl;
  dest.close();
  return;
}

int main(int argc, char *argv[]) {
  // load data from local folder
  // then begin to handle arguments
  
  // log args
  for (int i = 1; i < argc; i++) {
    cout << "argv[" << i << "]: " << argv[i] << endl;
  }

  json data = getInput();

  if (data.empty()) {
    cout << "Error: Invalid values.\n";
    // i'll bug the user to re-enter the data until they get it right later lol
    return 1;
  }

  writeJSON(data, "data");
  time_t curTime = time(NULL);

  // create a local folder to store
  // app configs, allowing for easy
  // management (like uninstallation)

  if (data["install"] == "true") {
    string name = data["name"];
    string platform = data["platform"];
    string dest = "~/.local/share/electrify/apps/" + name + "-" + platform + "_" + to_string(curTime) + "/";

    if (getOS() == "linux") {
      system(("mkdir -p " + dest).c_str());
    }

    // write the data file
    json config;

    config["name"] = name;
    config["description"] = data["description"];

    if (getOS() == "linux") {
      config["appmenu"] = data["appmenu"];
    } /*else if (getOS() == "windows") {
      config["start"] = data["start"];
    }*/

    config["desktop"] = data["desktop"];
    config["installed"] = to_string(curTime);

    writeJSON(config, dest + "config");

    // here we'll create shortcuts,
    // move em in the right places,
    // and bam! we're done! :D
  }

  // for testing the newly created data:
  system("npm run start");

  return 0;
}
