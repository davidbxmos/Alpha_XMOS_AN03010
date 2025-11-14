let port, writer, reader;
let keepReading = true;

async function connectSerial() {
  try {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });

    const decoder = new TextDecoderStream();
    port.readable.pipeTo(decoder.writable);
    reader = decoder.readable.getReader();

    const encoder = new TextEncoderStream();
    encoder.readable.pipeTo(port.writable);
    writer = encoder.writable.getWriter();

    log('Connected to serial port!');

    let buffer = '';
    keepReading = true;

    while (keepReading) {
      const { value, done } = await reader.read();
      if (done) break;
      if (value) {
        buffer += value;
        let lines = buffer.split(/\r?\n/);
        buffer = lines.pop();
        for (const line of lines) log('Received: ' + line.trim());
      }
    }
  } catch (err) {
    log('Error: ' + err);
  }
}

function disconnectSerial() {
  keepReading = false;
  if (reader) reader.cancel();
  if (writer) writer.close();
  if (port) port.close();
  log('Disconnected from serial port.');
}

async function sendMessage() {
  if (!writer) return log('Not connected.');
  const msg = document.getElementById('message').value;
  writer.write(msg + '\n');
  log('Sent: ' + msg);
  document.getElementById('message').value = '';
}

function log(text) {
  const el = document.getElementById('log');
  el.textContent += text + '\n';
  el.scrollTop = el.scrollHeight;
}

document.getElementById('connect').addEventListener('click', connectSerial);
document.getElementById('disconnect').addEventListener('click', disconnectSerial);
document.getElementById('send').addEventListener('click', sendMessage);
document.getElementById('message').addEventListener('keydown', (e) => {
  if (e.key === 'Enter') sendMessage();
});
