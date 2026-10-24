import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/routing/dev_router.dart';
import 'package:dimos/ui/core/themes/dash_palette.dart';
import 'package:dimos/ui/drive/popup_panel.dart';
import 'package:dimos/ui/fun/fun_overlays.dart';

/// The dashboard's screen: the current page plus anything drawn over it, laid
/// out at the design's 1280 × 480 and scaled to fit the display.
class DashboardFrame extends StatelessWidget {
  const DashboardFrame({super.key});

  static const Size designSize = Size(1280, 480);

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final router = context.watch<RouterProvider>();
    final fun = context.watch<FunEffects>();
    final shotNode = fun.shotNode;

    return FittedBox(
      child: SizedBox.fromSize(
        size: designSize,
        child: ClipRect(
          child: DefaultTextStyle(
            style: dashText(16, color: palette.foreground),
            child: ColoredBox(
              color: palette.background,
              child: Stack(
                fit: StackFit.expand,
                children: [
                  KeyedSubtree(key: ValueKey(router.index), child: router.currentPage.build()),
                  if (!router.isShowing(DashboardPages.drive)) const CenterPopup(),
                  if (fun.achievement) const AchievementToast(),
                  if (shotNode != null) FaultShot(key: ValueKey(shotNode), node: shotNode),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
