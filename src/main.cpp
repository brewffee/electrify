#include <iostream>
#include <fstream>

using namespace std;

int getInput() { 
  ofstream dest;
  string input, platform, install;

  dest.open("data.json");

  dest << "{\"name\":\"";
  cout << "Name: "; getline(cin, input);

  dest << input << "\",\"description\":\"";
  cout << "Description: "; getline(cin, input);
  
  dest << input << "\",\"url\":\"";  
  cout << "URL: "; getline(cin, input);
  
  dest << input << "\",\"platform\":\"";
  cout << "Platform: "; getline(cin, input);
  if (input != "windows" && input != "linux") {
    cout << "Invalid platform.\n";
    dest << "\"}";
    return 1;
  } else if (input == "mac") {
    cout << "Mac is not supported.\n";
    return 1;
  }
  platform = input;
  
  dest << input << "\",\"install\":\"";
  cout << "Would you like to install this as an app? (y/n): "; getline(cin, input);
  if (input == "y")  {
    install = "true";
    dest << "true\",\"start\":\"";
  } else {
    install = "false";
    dest << "false\"}";
  }
  
  // optional questions
  if (install == "true") {
    if (platform == "windows") {
      cout << "Would you like to add this to the start menu? (y/n): "; getline(cin, input);
    } else if (platform == "linux") {
      cout << "Would you like to add this to /usr/share/applications? (y/n): "; getline(cin, input);
    } 
    if (input == "y") {
      dest << "true\",\"desktop\":\"";
    } else {
      dest << "false\",\"desktop\":\"";
    }

    cout << "Would you like to add this to the desktop? (y/n): "; getline(cin, input);
    if (input == "y") {
      dest << "true\"}";
    } else {
      dest << "false\"}";
    }
  }

  dest.close();
  return 0;
}

int main() {
  getInput();
  // writeJSON();

  // we'll need to rewrite getInput() to give us the
  // variables we need to later create the json file and 
  // create the shortcuts and such.
  system("npm run start");
  return 0;
}
