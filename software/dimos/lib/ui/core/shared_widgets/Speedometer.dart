import 'package:flutter/material.dart';

class Speedometer extends StatelessWidget {
  const Speedometer({super.key});

  @override
  Widget build(BuildContext context) {
    return Column(mainAxisAlignment: MainAxisAlignment.center, children: [
      CircleAvatar(
        radius: 200,
        backgroundColor: Colors.blue,
      ),
      Text(
        "0",
        style: TextStyle(
            fontSize: 56, color: Colors.white, fontWeight: FontWeight.bold),
      ),
      Text(
        "km/h",
        style: TextStyle(fontSize: 16, color: Colors.white),
      ),
    ]);
  }

  // @override
  // State<StatefulWidget> createState() {
  //   throw UnimplementedError();
  // }
}
