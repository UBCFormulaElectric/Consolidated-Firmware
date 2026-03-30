import 'dart:async';
import 'dart:isolate';

class DevApiWorker {
  late SendPort _sendPort;

  Future<void> start(Function(String) onDataReceived) async {
    final receivePort = ReceivePort();
    await Isolate.spawn(_worker, receivePort.sendPort);

    receivePort.listen((message) {
      if (message is SendPort) {
        _sendPort = message;
      } else if (message is String) {
        onDataReceived(message); 
      }
    });
  }

  static void _worker(SendPort mainSendPort) {
    final workerPort = ReceivePort();
    mainSendPort.send(workerPort.sendPort);

    Timer.periodic(Duration(seconds: 5), (timer) {
      final newData = "W";
      mainSendPort.send(newData);
    });
  }
}