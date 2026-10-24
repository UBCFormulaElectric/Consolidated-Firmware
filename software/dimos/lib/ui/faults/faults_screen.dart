import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'package:dimos/data/models/display_settings.dart';
import 'package:dimos/data/models/vehicle_state.dart';
import 'package:dimos/data/services/can_signals.dart';
import 'package:dimos/ui/core/shared_widgets/effects.dart';
import 'package:dimos/ui/core/themes/dash_palette.dart';

/// Node / board health: the shutdown loop drawn as a ring around the boards.
/// The first open node flashes; nodes after it in the car's loop are greyed
/// out, since they're open because of it. Parts of the loop with no CAN status
/// are drawn as outlines.
class FaultsScreen extends StatelessWidget {
  const FaultsScreen({super.key});

  // Two nodes down each side, the rest split across the top and bottom.
  static const int _sideCount = 2;

  @override
  Widget build(BuildContext context) {
    final vehicle = context.watch<VehicleState>();
    final party = context.select<FunEffects, bool>((f) => f.party);
    final first = vehicle.firstFaultNode;

    final nodes = [
      for (final (i, tile) in shutdownRing.indexed)
        _NodeTile(index: i, tile: tile, firstFault: first, party: party),
    ];
    final topCount = (nodes.length - 2 * _sideCount + 1) ~/ 2;
    final top = nodes.sublist(0, topCount);
    final right = nodes.sublist(topCount, topCount + _sideCount);
    final bottom = nodes.sublist(topCount + _sideCount, nodes.length - _sideCount).reversed;
    final left = nodes.sublist(nodes.length - _sideCount).reversed;

    Widget row(Iterable<Widget> tiles) => SizedBox(
          height: 46,
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            spacing: 8,
            children: [for (final tile in tiles) Expanded(child: tile)],
          ),
        );
    Widget column(Iterable<Widget> tiles) => SizedBox(
          width: 124,
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            spacing: 8,
            children: [for (final tile in tiles) Expanded(child: tile)],
          ),
        );

    final boards = vehicle.boards;
    return Padding(
      padding: const EdgeInsets.all(16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        spacing: 10,
        children: [
          SizedBox(height: 46, child: _Header(vehicle)),
          row(top),
          Expanded(
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.stretch,
              spacing: 8,
              children: [
                column(left),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.stretch,
                    spacing: 8,
                    children: [
                      for (var r = 0; r < 2; r++)
                        Expanded(
                          child: Row(
                            crossAxisAlignment: CrossAxisAlignment.stretch,
                            spacing: 8,
                            children: [
                              for (final (c, status) in boards.skip(r * 3).take(3).indexed)
                                Expanded(
                                    child: _BoardTile(status, party: party, seed: 30 + r * 3 + c)),
                            ],
                          ),
                        ),
                    ],
                  ),
                ),
                column(right),
              ],
            ),
          ),
          row(bottom),
        ],
      ),
    );
  }
}

class _Header extends StatelessWidget {
  const _Header(this.vehicle);

  final VehicleState vehicle;

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final first = vehicle.firstFaultNode;
    final chips = [
      if (first != null) (carShutdownLoop[first].name, 'LOOP OPEN · FIRST'),
      for (final board in vehicle.faultedBoards) (board.board.label, board.fault!),
    ];
    final count = vehicle.faultCount;
    return Row(
      spacing: 12,
      children: [
        Container(
          color: palette.bad,
          padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 8),
          child: Text(count > 0 ? 'FAULT' : 'NO FAULTS', style: dashText(20, color: Colors.white)),
        ),
        Expanded(
          child: ClipRect(
            child: OverflowBox(
              alignment: Alignment.centerLeft,
              maxWidth: double.infinity,
              child: Row(
                spacing: 12,
                children: [
                  for (final (name, message) in chips)
                    Container(
                      color: palette.cell,
                      padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
                      child: Row(
                        spacing: 8,
                        children: [
                          Text(name, style: dashText(15, color: palette.bad)),
                          Text(message,
                              style: dashText(15, weight: FontWeight.w800, color: palette.foreground)),
                        ],
                      ),
                    ),
                ],
              ),
            ),
          ),
        ),
        Text(
          count > 0 ? '$count ACTIVE · PAGE TO RETURN' : 'ALL NOMINAL · PAGE TO RETURN',
          style: dashText(16, weight: FontWeight.w800),
        ),
      ],
    );
  }
}

class _NodeTile extends StatelessWidget {
  const _NodeTile({
    required this.index,
    required this.tile,
    required this.firstFault,
    required this.party,
  });

  /// Position in [shutdownRing].
  final int index;
  final RingTile tile;

  /// Index into [carShutdownLoop].
  final int? firstFault;
  final bool party;

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final node = tile.node;
    final first = firstFault;
    final isFirst = node != null && node == first;
    final isDown = node != null && first != null && node > first;
    final foreground = isFirst
        ? (party ? Colors.black : Colors.white)
        : isDown || node == null
            ? palette.downNodeText
            : palette.foreground;
    final source = node == null
        ? 'NO CAN'
        : isFirst
            ? 'FIRST OPEN'
            : carShutdownLoop[node].reportedBy.label;

    final label = Padding(
      padding: const EdgeInsets.symmetric(horizontal: 4),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        spacing: 1,
        children: [
          Text(tile.name,
              maxLines: 1,
              overflow: TextOverflow.ellipsis,
              style: dashText(14, weight: FontWeight.w800, color: foreground)),
          Text(
            '${'${index + 1}'.padLeft(2, '0')} · $source',
            maxLines: 1,
            overflow: TextOverflow.ellipsis,
            style: dashText(11, weight: FontWeight.w700, color: foreground),
          ),
        ],
      ),
    );

    if (node == null) {
      return DecoratedBox(
        decoration: BoxDecoration(border: Border.all(color: palette.off, width: 2)),
        child: label,
      );
    }
    if (isFirst && party) return Rainbow(seed: index, child: label);
    final box = Stack(
      fit: StackFit.expand,
      children: [
        ColoredBox(color: isFirst ? palette.bad : isDown ? palette.downNode : palette.cell),
        if (isDown) const Stripes(color: Color(0x4D000000), width: 6, period: 14),
        label,
      ],
    );
    return isFirst ? Blink(child: box) : box;
  }
}

class _BoardTile extends StatelessWidget {
  const _BoardTile(this.status, {required this.party, required this.seed});

  final BoardStatus status;
  final bool party;
  final int seed;

  @override
  Widget build(BuildContext context) {
    final palette = DashPalette.of(context);
    final fault = status.fault;
    final foreground = party
        ? Colors.black
        : fault != null
            ? Colors.white
            : palette.foreground;
    final statusColor = party
        ? Colors.black
        : fault != null
            ? Colors.white
            : palette.ok;
    final vitalsId = status.vitalsId;

    final content = Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            crossAxisAlignment: CrossAxisAlignment.baseline,
            textBaseline: TextBaseline.alphabetic,
            children: [
              Text(status.board.label, style: dashText(22, color: foreground)),
              if (vitalsId != null)
                Text('0x${vitalsId.toRadixString(16).toUpperCase()}',
                    style: dashText(14, weight: FontWeight.w800, color: foreground)),
            ],
          ),
          Row(
            crossAxisAlignment: CrossAxisAlignment.baseline,
            textBaseline: TextBaseline.alphabetic,
            spacing: 8,
            children: [
              Expanded(
                child: Text(fault ?? status.board.description,
                    maxLines: 1,
                    overflow: TextOverflow.ellipsis,
                    style: dashText(15, weight: FontWeight.w800, color: foreground)),
              ),
              Text(fault != null ? 'FAULT' : 'OK', style: dashText(15, color: statusColor)),
            ],
          ),
        ],
      ),
    );

    if (party) return Rainbow(seed: seed, child: content);
    final tile = ColoredBox(color: fault != null ? palette.bad : palette.cell, child: content);
    return fault != null ? Blink(child: tile) : tile;
  }
}
