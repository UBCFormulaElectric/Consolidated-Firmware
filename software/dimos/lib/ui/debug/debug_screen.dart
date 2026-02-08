/* Debug Screen */

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:dimos/data/services/can_variables.dart';

class DebugScreen extends StatelessWidget {
  const DebugScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final debugVars = context.watch<DebugVars>().debugVars;

    return Scaffold(
      backgroundColor: const Color(0xFF121212),
      body: SafeArea(
        child: Padding(
          padding: const EdgeInsets.all(24),
          child: Row(
            children: [
              // ===== LEFT: CAR + SENSORS =====
              Expanded(
                flex: 4,
                child: Stack(
                  alignment: Alignment.center,
                  children: [
                    // Car silhouette placeholder
                    Container(
                      width: 220,
                      height: 420,
                      decoration: BoxDecoration(
                        color: Colors.white.withOpacity(0.9),
                        borderRadius: BorderRadius.circular(16),
                      ),
                    ),

                    // Left sensors
                    Positioned(left: 0, child: _sensorBar()),
                    Positioned(left: 0, bottom: 0, child: _sensorBar()),

                    // Right sensors
                    Positioned(right: 0, child: _sensorBar()),
                    Positioned(right: 0, bottom: 0, child: _sensorBar()),
                  ],
                ),
              ),

              const SizedBox(width: 32),

              // ===== RIGHT: INFO PANEL =====
              Expanded(
                flex: 5,
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    // GPS header
                    Row(
                      children: const [
                        Icon(Icons.gps_fixed, color: Colors.white, size: 28),
                        SizedBox(width: 12),
                        Text(
                          'GPS',
                          style: TextStyle(
                            color: Colors.white,
                            fontSize: 36,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                    const SizedBox(height: 12),

                    // GPS variables (your existing list)
                    ...debugVars.map(
                      (v) => Padding(
                        padding: const EdgeInsets.only(bottom: 6),
                        child: Text(
                          v,
                          style: const TextStyle(
                            color: Colors.white70,
                            fontSize: 18,
                          ),
                        ),
                      ),
                    ),

                    const SizedBox(height: 32),

                    // Settings header
                    Row(
                      children: const [
                        Icon(Icons.settings, color: Colors.white, size: 28),
                        SizedBox(width: 12),
                        Text(
                          'Settings',
                          style: TextStyle(
                            color: Colors.white,
                            fontSize: 36,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                    const SizedBox(height: 12),

                    const Text(
                      'traction control level',
                      style: TextStyle(color: Colors.white70, fontSize: 18),
                    ),
                    const SizedBox(height: 8),

                    const Text(
                      'TODO finalize settings',
                      style: TextStyle(color: Colors.white38, fontSize: 16),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  static Widget _sensorBar() {
    return Container(
      width: 18,
      height: 120,
      decoration: BoxDecoration(
        color: Colors.greenAccent,
        borderRadius: BorderRadius.circular(6),
      ),
    );
  }
}
