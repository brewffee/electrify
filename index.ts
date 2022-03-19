import { spawn } from 'child_process';

const child = spawn('./main', process.argv.splice(2), {
    stdio: ['inherit', 'inherit', 'inherit', 'ipc']
});
