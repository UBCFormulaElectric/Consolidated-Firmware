import 'dart:async';

import 'package:flutter/foundation.dart';

import 'package:dimos/data/input/input_service.dart';
import 'package:dimos/data/models/info_feed.dart';
import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/routing/dev_router.dart';
import 'package:dimos/routing/input_router.dart';

class DriveMode {
  const DriveMode(this.name, this.short, {this.torqueVectoring});

  final String name;
  final String short;

  /// The torque vectoring level the mode sets, or null if it doesn't use TV
  /// (and so stays selectable with TV off).
  final int? torqueVectoring;
}

const List<DriveMode> driveModes = [
  DriveMode('VANILLA', 'VAN'),
  DriveMode('TRACK', 'TRK', torqueVectoring: 7),
  DriveMode('ACCEL', 'ACC', torqueVectoring: 4),
];

enum PopupKind { torqueVectoring, regen, mode }

class Popup {
  const Popup(this.title, this.value, this.hint, this.kind);

  final String title;
  final String value;
  final String hint;
  final PopupKind kind;
}

/// The driver's settings on the wheel: torque vectoring and regen strength
/// (rotary turns the level, push-and-hold toggles it), and the drive mode
/// (rotary opens and moves a selector, push confirms). Settings only change
/// while the car is [VehicleState.live]; otherwise a LOCKED popup explains why.
///
/// PLACEHOLDER: DIMOS doesn't transmit on CAN yet, so these settings only live
/// on the dashboard. Send them once DIMOS_tx.json has messages for them.
class DriverControls extends ChangeNotifier implements InputMiddleware {
  DriverControls(this._vehicle, this._router, this._info) {
    _vehicle.addListener(_onVehicleChanged);
  }

  static const int minLevel = 1;
  static const int maxLevel = 10;
  static const Duration holdToToggle = Duration(seconds: 1);
  static const Duration selectorTimeout = Duration(seconds: 8);

  final VehicleState _vehicle;
  final RouterProvider _router;
  final InfoFeed _info;

  int _torqueVectoring = 5;
  int _regen = 5;
  bool _torqueVectoringOn = true;
  bool _regenOn = true;
  int _mode = 0;
  int? _selection;
  Popup? _popup;

  Timer? _popupTimer;
  Timer? _selectorTimer;
  Timer? _holdTimer;

  int get torqueVectoring => _torqueVectoring;
  int get regen => _regen;
  bool get torqueVectoringOn => _torqueVectoringOn;
  bool get regenOn => _regenOn;
  DriveMode get mode => driveModes[_mode];
  int get modeIndex => _mode;

  /// The highlighted row while the drive mode selector is open.
  int? get selection => _selection;
  Popup? get popup => _popup;

  /// How hard braking regenerates (0-1), for VehicleSim.
  double get regenStrength => _regenOn ? 0.2 + _regen * 0.06 : 0;

  bool isModeLocked(DriveMode mode) =>
      !_torqueVectoringOn && mode.torqueVectoring != null;

  @override
  bool handleInput(DashboardInput input) {
    switch (input) {
      case ButtonPressed(button: DashboardButton.left || DashboardButton.right):
        // Paging closes whatever was open; the router still gets the press.
        _closeOverlays();
        return false;
      case ButtonPressed(button: DashboardButton.ok):
        if (_selection != null) {
          _confirmMode();
        } else {
          _setPopup(null);
        }
      case ButtonPressed(button: DashboardButton.telem):
        _info.add('DASH', 'TELEM', InfoStyle.telem);
      case ButtonPressed(button: DashboardButton.driveModePush):
        _confirmMode();
      case ButtonPressed(button: DashboardButton.torqueVectoringPush):
        _startHold(PopupKind.torqueVectoring);
      case ButtonPressed(button: DashboardButton.regenPush):
        _startHold(PopupKind.regen);
      case ButtonReleased(
          button: DashboardButton.torqueVectoringPush || DashboardButton.regenPush
        ):
        _holdTimer?.cancel();
      case RotaryTurned(rotary: DashboardRotary.torqueVectoring, :final steps):
        _turnLevel(PopupKind.torqueVectoring, steps);
      case RotaryTurned(rotary: DashboardRotary.regen, :final steps):
        _turnLevel(PopupKind.regen, steps);
      case RotaryTurned(rotary: DashboardRotary.driveMode, :final steps):
        _turnModeSelector(steps);
      default:
        return false;
    }
    return true;
  }

  /// Closes the selector and popup, e.g. when something else takes the screen.
  void _closeOverlays() {
    _selectorTimer?.cancel();
    _holdTimer?.cancel();
    _selection = null;
    _setPopup(null);
  }

  static String _title(PopupKind kind) =>
      kind == PopupKind.torqueVectoring ? 'TORQUE VECTORING' : 'REGEN STRENGTH';

  bool _checkLive(String title) {
    if (_vehicle.live) return true;
    _router.show(DashboardPages.drive);
    _selection = null;
    _setPopup(
      Popup(title, 'LOCKED',
          _vehicle.hasFault ? 'DISABLED IN FAULT' : 'DISABLED IN INIT/LV', PopupKind.mode),
      const Duration(seconds: 2),
    );
    return false;
  }

  void _turnLevel(PopupKind kind, int steps) {
    if (!_checkLive(_title(kind))) return;
    final int level;
    if (kind == PopupKind.torqueVectoring) {
      level = _torqueVectoring = (_torqueVectoring + steps).clamp(minLevel, maxLevel);
      _torqueVectoringOn = true;
    } else {
      level = _regen = (_regen + steps).clamp(minLevel, maxLevel);
      _regenOn = true;
    }
    _router.show(DashboardPages.drive);
    _selection = null;
    _setPopup(Popup(_title(kind), '$level', 'PRESS ANY TO DISMISS', kind),
        const Duration(seconds: 3));
  }

  void _startHold(PopupKind kind) {
    _holdTimer?.cancel();
    _holdTimer = Timer(holdToToggle, () => _toggle(kind));
  }

  void _toggle(PopupKind kind) {
    if (!_checkLive(_title(kind))) return;
    final bool on;
    final int level;
    if (kind == PopupKind.torqueVectoring) {
      on = _torqueVectoringOn = !_torqueVectoringOn;
      level = _torqueVectoring;
      if (!on && mode.torqueVectoring != null) {
        _mode = 0;
        _info.add('VC', 'TV OFF → VANILLA', InfoStyle.system);
      }
    } else {
      on = _regenOn = !_regenOn;
      level = _regen;
    }
    _router.show(DashboardPages.drive);
    _selection = null;
    _setPopup(Popup(_title(kind), on ? '$level' : 'OFF', on ? 'ENABLED' : 'DISABLED', kind),
        const Duration(seconds: 2));
  }

  void _turnModeSelector(int steps) {
    if (!_checkLive('DRIVE MODE')) return;
    _holdTimer?.cancel();
    _selectorTimer?.cancel();
    _selectorTimer = Timer(selectorTimeout, () {
      _selection = null;
      notifyListeners();
    });
    final current = _selection;
    if (current == null) {
      _selection = _mode;
    } else {
      var next = current;
      do {
        next = (next + steps.sign) % driveModes.length;
      } while (isModeLocked(driveModes[next]) && next != current);
      _selection = next;
    }
    _router.show(DashboardPages.drive);
    _setPopup(null);
  }

  void _confirmMode() {
    final selection = _selection;
    if (!_vehicle.live || selection == null) return;
    final chosen = driveModes[selection];
    if (isModeLocked(chosen)) return;
    _selectorTimer?.cancel();
    _holdTimer?.cancel();
    if (selection != _mode) _info.add('', 'MODE → ${chosen.short}', InfoStyle.confirm);
    _mode = selection;
    _selection = null;
    if (chosen.torqueVectoring != null) {
      _torqueVectoringOn = true;
      _torqueVectoring = chosen.torqueVectoring!;
    }
    notifyListeners();
  }

  void _setPopup(Popup? popup, [Duration? showFor]) {
    _popupTimer?.cancel();
    _popup = popup;
    if (popup != null && showFor != null) {
      _popupTimer = Timer(showFor, () {
        _popup = null;
        notifyListeners();
      });
    }
    notifyListeners();
  }

  // Leaving RTD/DRIVE (or faulting) drops back to VANILLA.
  void _onVehicleChanged() {
    if (_vehicle.live || (_mode == 0 && _selection == null)) return;
    _selectorTimer?.cancel();
    _mode = 0;
    _selection = null;
    notifyListeners();
  }

  @override
  void dispose() {
    _vehicle.removeListener(_onVehicleChanged);
    _popupTimer?.cancel();
    _selectorTimer?.cancel();
    _holdTimer?.cancel();
    super.dispose();
  }
}
