import 'package:flutter/foundation.dart';

import 'package:dimos/data/input/input_service.dart';

/// One stage of input routing. Returns true if it consumed [input], which stops
/// it reaching later stages.
abstract interface class InputMiddleware {
  bool handleInput(DashboardInput input);
}

/// Passes each wheel input down a chain of [InputMiddleware], most specific
/// first (e.g. a game that has taken over the controls), page navigation last.
class InputRouter {
  InputRouter(this._stages);

  final List<InputMiddleware> _stages;

  void route(DashboardInput input) {
    for (final stage in _stages) {
      if (stage.handleInput(input)) return;
    }
    if (input is! ButtonReleased) debugPrint('Input unhandled: $input');
  }
}
