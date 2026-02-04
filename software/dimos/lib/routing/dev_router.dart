import 'package:dimos/ui/braking/braking_screen.dart';
import 'package:dimos/ui/debug/debug_screen.dart';
import 'package:dimos/ui/low_voltage/lv_screen.dart';
import 'package:dimos/ui/racing/racing_screen.dart';
import 'package:dimos/ui/skidpad/skidpad_screen.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

// Make a better router which hides/reveals onkeypressed
// Not exactly sure about design for now

class RouterProvider extends ChangeNotifier {
  Widget _currentPage = LVScreen();

  Widget get currentPage => _currentPage;

  void navigateTo(Widget page) {
    _currentPage = page;
    notifyListeners(); // Triggers UI update
  }
}

class RouterPanel extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Container(
      color: const Color.fromARGB(255, 0, 0, 0), // Background color for the router
      child: Column(
        children: [
          ListTile(
            title: Text("LV Page"),
            onTap: () => context.read<RouterProvider>().navigateTo(LVScreen()),
          ),
          ListTile(
            title: Text("Racing Page"),
            onTap: () => context.read<RouterProvider>().navigateTo(RacingScreen()),
          ),
          ListTile(
            title: Text("Skidpad Page"),
            onTap: () => context.read<RouterProvider>().navigateTo(SkidpadScreen()),
          ),
          ListTile(
            title: Text("Braking Page"),
            onTap: () => context.read<RouterProvider>().navigateTo(BrakingScreen()),
          ),
          ListTile(
            title: Text("Debug Page"),
            onTap: () => context.read<RouterProvider>().navigateTo(DebugScreen()),
          ),
        ],
      ),
    );
  }
}