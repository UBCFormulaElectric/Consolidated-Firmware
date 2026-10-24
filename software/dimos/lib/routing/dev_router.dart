import 'package:flutter/material.dart';

import 'package:dimos/data/input/input_service.dart';
import 'package:dimos/routing/input_router.dart';
import 'package:dimos/ui/drive/drive_screen.dart';
import 'package:dimos/ui/faults/faults_screen.dart';

class DashboardPage {
  const DashboardPage(this.name, this.build);

  final String name;
  final Widget Function() build;
}

abstract final class DashboardPages {
  static final drive = DashboardPage('DRIVE', () => const DriveScreen());
  static final faults = DashboardPage('NODE / BOARD HEALTH', () => const FaultsScreen());
}

// The pages in the order the left/right buttons move through them.
final List<DashboardPage> dashboardPages = [
  DashboardPages.drive,
  DashboardPages.faults,
];

/// The last stage of input routing: left/right page through [dashboardPages],
/// wrapping around at either end. Other code jumps straight to a page with [show].
class RouterProvider extends ChangeNotifier implements InputMiddleware {
  int _index = 0;

  int get index => _index;
  DashboardPage get currentPage => dashboardPages[_index];
  bool isShowing(DashboardPage page) => currentPage == page;

  void next() => _goTo((_index + 1) % dashboardPages.length);

  void previous() => _goTo((_index - 1) % dashboardPages.length);

  void show(DashboardPage page) => _goTo(dashboardPages.indexOf(page));

  @override
  bool handleInput(DashboardInput input) {
    switch (input) {
      case ButtonPressed(button: DashboardButton.left):
        previous();
      case ButtonPressed(button: DashboardButton.right):
        next();
      default:
        return false;
    }
    return true;
  }

  void _goTo(int index) {
    if (index == _index) return;
    _index = index;
    notifyListeners();
  }
}
