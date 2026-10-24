import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/models/driver_controls.dart';
import 'package:dimos/data/models/info_feed.dart';
import 'package:dimos/data/models/snake_game.dart';
import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/data/services/can_signals.dart';
import 'package:dimos/ui/core/shared_widgets/effects.dart';
import 'package:dimos/ui/core/themes/dash_palette.dart';
import 'package:dimos/ui/drive/popup_panel.dart';
import 'package:dimos/ui/fun/snake_view.dart';

/// The main racing page: SoC, power/regen, speed, pedals, state, driver settings.
class DriveScreen extends StatelessWidget {
  const DriveScreen({super.key});

  @override
  Widget build(BuildContext context) => const Padding(
        padding: EdgeInsets.all(18),
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          spacing: 22,
          children: [
            SizedBox(width: 76, child: _SocColumn()),
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.stretch,
                spacing: 14,
                children: [
                  SizedBox(height: 44, child: _PowerSegments()),
                  Expanded(
                    child: Row(
                      crossAxisAlignment: CrossAxisAlignment.stretch,
                      spacing: 14,
                      children: [
                        Expanded(child: _InfoColumn()),
                        SizedBox(width: 470, child: _SpeedBox()),
                        Expanded(child: SidePanel()),
                      ],
                    ),
                  ),
                  SizedBox(height: 56, child: _Pedals()),
                  SizedBox(height: 26, child: _StateBar()),
                ],
              ),
            ),
            SizedBox(width: 176, child: _SettingTiles()),
          ],
        ),
      );
}

const double lowSocPercent = 20;

class _SocColumn extends StatelessWidget {
  const _SocColumn();

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final soc = context.select<VehicleState, double>((v) => v.socPercent);
    final color = soc < lowSocPercent ? palette.red : palette.green;
    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      spacing: 10,
      children: [
        Expanded(
          child: ColoredBox(
            color: palette.off,
            child: Align(
              alignment: Alignment.bottomCenter,
              child: FractionallySizedBox(
                heightFactor: (soc / 100).clamp(0, 1),
                widthFactor: 1,
                child: ColoredBox(color: color),
              ),
            ),
          ),
        ),
        FittedBox(
          fit: BoxFit.scaleDown,
          child: Text('${soc.round()}%', style: dashText(32)),
        ),
      ],
    );
  }
}

class _PowerSegments extends StatelessWidget {
  const _PowerSegments();

  static const int powerSegments = 20;
  static const int regenSegments = 6;

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final power = context.select<VehicleState, double>((v) => v.powerPercent);
    final powerLit = (power.clamp(0, 100) / 100 * powerSegments).round();
    // Regen tops out well below full power, so it gets a steeper scale.
    final regenLit = ((-power * 3).clamp(0, 100) / 100 * regenSegments).round();

    Widget dot(Color color) => Container(
          width: 30,
          height: 30,
          decoration: BoxDecoration(color: color, shape: BoxShape.circle),
        );

    return Center(
      child: SizedBox(
        height: 36,
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            for (var i = 0; i < regenSegments; i++)
              dot(i >= regenSegments - regenLit ? palette.regen : palette.off),
            Container(width: 3, color: palette.foreground),
            for (var i = 0; i < powerSegments; i++)
              dot(i >= powerLit
                  ? palette.off
                  : i < 12
                      ? palette.green
                      : i < 17
                          ? palette.yellow
                          : palette.red),
          ],
        ),
      ),
    );
  }
}

class _InfoColumn extends StatelessWidget {
  const _InfoColumn();

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final lowSoc = context.select<VehicleState, bool>((v) => v.socPercent < lowSocPercent);
    final messages = [
      if (lowSoc) const InfoMessage('BMS', 'LOW SOC', InfoStyle.warning),
      ...context.watch<InfoFeed>().shown,
    ];
    final shown = messages.sublist(
        messages.length > InfoFeed.maxShown ? messages.length - InfoFeed.maxShown : 0);

    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      spacing: 6,
      children: [
        for (final info in shown)
          Container(
            height: 50,
            padding: const EdgeInsets.symmetric(horizontal: 12),
            color: switch (info.style) {
              InfoStyle.warning => palette.yellow,
              InfoStyle.confirm => Colors.white,
              InfoStyle.telem => palette.pink,
              InfoStyle.system => palette.mode,
            },
            child: Row(
              spacing: 10,
              children: [
                if (info.style == InfoStyle.confirm)
                  Container(
                    width: 24,
                    height: 24,
                    alignment: Alignment.center,
                    decoration:
                        const BoxDecoration(color: Colors.black, shape: BoxShape.circle),
                    child: Text('i',
                        style: dashText(17, color: Colors.white)
                            .copyWith(fontStyle: FontStyle.italic, fontFamily: 'Georgia')),
                  ),
                if (info.source.isNotEmpty)
                  Text(info.source, style: dashText(14, weight: FontWeight.w800, color: Colors.black)),
                Expanded(
                  child: Text(
                    info.message,
                    maxLines: 1,
                    overflow: TextOverflow.ellipsis,
                    style: dashText(18, color: Colors.black),
                  ),
                ),
              ],
            ),
          ),
      ],
    );
  }
}

class _SpeedBox extends StatelessWidget {
  const _SpeedBox();

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final speed = context.select<VehicleState, int>((v) => v.speedKph.round());
    final hasFault = context.select<VehicleState, bool>((v) => v.hasFault);
    final driveFlash = context.select<VehicleState, bool>((v) => v.driveFlash);
    final estopOnly = context.select<VehicleState, bool>((v) => v.estopOnly);
    final snake = context.select<SnakeGame, bool>((s) => s.running);

    return DecoratedBox(
      decoration: BoxDecoration(border: Border.all(color: palette.foreground, width: 4)),
      child: ClipRect(
        child: Stack(
          fit: StackFit.expand,
          children: [
            if (hasFault) const Stripes(color: Color(0x61E60000)),
            if (driveFlash)
              const Blink(
                period: Duration(milliseconds: 450),
                repeat: 3,
                dimmed: 0,
                startDimmed: true,
                child: Stripes(color: Color(0x57FFD200)),
              ),
            Center(
              child: Text('$speed',
                  style: dashText(220).copyWith(letterSpacing: -0.04 * 220, height: 0.78)),
            ),
            Positioned(
              right: 12,
              bottom: 8,
              child: Text('KM/H', style: dashText(20, weight: FontWeight.w800)),
            ),
            if (estopOnly) const Positioned(left: 0, right: 0, top: 0, child: _EstopBanner()),
            if (snake) const SnakeView(),
          ],
        ),
      ),
    );
  }
}

class _EstopBanner extends StatelessWidget {
  const _EstopBanner();

  @override
  Widget build(BuildContext context) {
    final first = context.select<VehicleState, int?>((v) => v.firstFaultNode);
    final name = first == null ? '' : carShutdownLoop[first].name.replaceAll(' ESTOP', '');
    return Container(
      height: 40,
      color: DashPalette.of(context).bad,
      padding: const EdgeInsets.symmetric(horizontal: 10),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        spacing: 10,
        children: [
          Text('RELEASE ESTOP', style: dashText(14, weight: FontWeight.w800, color: Colors.white)),
          Text(name, style: dashText(18, color: Colors.white)),
        ],
      ),
    );
  }
}

class _Pedals extends StatelessWidget {
  const _Pedals();

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final brake = context.select<VehicleState, int>((v) => v.brakePercent.round());
    final throttle = context.select<VehicleState, int>((v) => v.throttlePercent.round());

    Widget bar(String label, int value, Color color) => Expanded(
          child: Row(
            spacing: 12,
            children: [
              SizedBox(width: 52, child: Text(label, style: dashText(20))),
              Expanded(
                child: Container(
                  height: 24,
                  color: palette.off,
                  alignment: Alignment.centerLeft,
                  child: FractionallySizedBox(
                    widthFactor: value / 100,
                    heightFactor: 1,
                    child: ColoredBox(color: color),
                  ),
                ),
              ),
              SizedBox(
                width: 44,
                child: Text('$value', textAlign: TextAlign.right, style: dashText(22)),
              ),
            ],
          ),
        );

    return Row(
      spacing: 22,
      children: [
        bar('BRK', brake, palette.brake),
        bar('THR', throttle, palette.throttle),
      ],
    );
  }
}

class _StateBar extends StatelessWidget {
  const _StateBar();

  static const Map<SystemState, String> labels = {
    SystemState.init: 'INIT / LV',
    SystemState.readyToDrive: 'READY TO DRIVE',
    SystemState.drive: 'DRIVE',
    SystemState.fault: 'FAULT',
  };

  @override
  Widget build(BuildContext context) {
    final system = context.select<VehicleState, SystemState>((v) => v.system);
    final (background, foreground) = DashPalette.of(context).states[system]!;
    return Container(
      color: background,
      alignment: Alignment.center,
      child: Text(labels[system]!, style: dashText(18, color: foreground)),
    );
  }
}

class _SettingTiles extends StatelessWidget {
  const _SettingTiles();

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final live = context.select<VehicleState, bool>((v) => v.live);
    final controls = context.watch<DriverControls>();
    final tvOn = live && controls.torqueVectoringOn;
    final regenOn = live && controls.regenOn;

    Widget tile(String label, String value, Color background, Color foreground,
            {double valueSize = 60}) =>
        Expanded(
          child: Container(
            color: background,
            padding: const EdgeInsets.symmetric(horizontal: 18),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text(label, style: dashText(22, color: foreground)),
                Text(value, style: dashText(valueSize, color: foreground)),
              ],
            ),
          ),
        );

    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      spacing: 6,
      children: [
        tile('TV', tvOn ? '${controls.torqueVectoring}' : 'OFF',
            tvOn ? palette.torqueVectoring : palette.disabled, Colors.white),
        tile('RS', regenOn ? '${controls.regen}' : 'OFF',
            regenOn ? palette.regenStrength : palette.disabled, Colors.white),
        tile('MODE', live ? controls.mode.short : driveModes.first.short, palette.mode,
            Colors.black,
            valueSize: 56),
      ],
    );
  }
}
