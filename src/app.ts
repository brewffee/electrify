import { exec } from 'child_process';
import { app, BrowserWindow } from 'electron';

import * as json from './data.json';

interface AppData {
  name: string;
  description: string;
  url: string;
  platform: "windows" | "linux" | "mac";
  install: "true" | "false";
  start: "true" | "false";
  desktop: "true" | "false";
}

const data = json as AppData;

function init() {
  let url: string;
  if (!/^https?:\/\//.test(data.url)) url = "http://" + data.url;
  else url = data.url;

  new BrowserWindow({
    width: 800,
    height: 600,
    autoHideMenuBar: true,
    title: data?.name || "Electrify",
  }).loadURL(url)
}

app.whenReady().then(() => {
  init();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) init();
  })
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
})

exec('npm run package');

if (data.install) {
  // if linux, move packaged file to opt/electrify/appname
  if (data.platform === "linux") {
    exec(`mkdir -p /opt/electrify/${data.name}`);
    exec(`mv out/electrify-${data.platform}-x64/${data.name} /opt/electrify/${data.name}`);
  }
  // if windows, move packaged file to C:\Users\username\AppData\Roaming\electrify\appname
  else if (data.platform === "windows") {
    exec(`mkdir -p %APPDATA%\\electrify\\${data.name}`);
    exec(`mv out/electrify-${data.platform}-x64\\${data.name} %APPDATA%\\electrify\\${data.name}`);
  }
} else if (data.start) {
  // if linux, create a .desktop file in /usr/share/applications
  if (data.platform === "linux") {
    exec(`echo "[Desktop Entry]
Name=${data.name}
Comment=${data.description}
Exec=out/electrify-${data.platform}-x64/${data.name}
Icon=out/electrify-${data.platform}-x64/${data.name}/icon.png
Terminal=false
Type=Application" > /usr/share/applications/${data.name}.desktop`);
  }
  // if windows, create a .lnk file in the start menu
  else if (data.platform === "windows") {
    // TODO: create .lnk file in start menu
  }
} else if (data.desktop) {
  // if linux, create a .desktop file in /home/username/Desktop
  if (data.platform === "linux") {
    exec(`echo "[Desktop Entry]
Name=${data.name}
Comment=${data.description}
Exec=out/electrify-${data.platform}-x64/${data.name}
Icon=out/electrify-${data.platform}-x64/${data.name}/icon.png
Terminal=false
Type=Application" > /home/username/Desktop/${data.name}.desktop`);
  }
  else if (data.platform === "windows") {
    // Create .lnk file and store it in C:\Users\username\Desktop
    exec(`echo "[Desktop Entry]
Name=${data.name}
Comment=${data.description}
Exec=out/electrify-${data.platform}-x64\\${data.name}
Icon=out/electrify-${data.platform}-x64\\${data.name}\\icon.png
Terminal=false
Type=Application" > C:\\Users\\username\\Desktop\\${data.name}.desktop`);
  }
}