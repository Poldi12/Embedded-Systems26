from flask import Flask, render_template, request
from flask_socketio import SocketIO

app = Flask(__name__)

socketio = SocketIO(app, cors_allowed_origins="*", async_mode='threading')


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/data', methods=['POST'])
def receive_data():

    data = request.json

    print(data)

    socketio.emit('new_data', data)

    return {"status": "ok"}


if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000)
