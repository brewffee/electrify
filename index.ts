import { spawn } from 'child_process';

const child = spawn('./main', [], {
    stdio: ['inherit', 'inherit', 'inherit', 'ipc']
});
