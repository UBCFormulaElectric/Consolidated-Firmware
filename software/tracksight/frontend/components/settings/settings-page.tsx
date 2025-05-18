"use client"

import { useState } from "react"
import { Card, CardContent, CardHeader, CardTitle, CardDescription } from "@/components/ui/card"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import { Input } from "@/components/ui/input"
import { Label } from "@/components/ui/label"
import { Switch } from "@/components/ui/switch"
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select"

export default function SettingsPage() {
  const [activeTab, setActiveTab] = useState("general")

  return (
    <div className="p-6 text-gray-800 dark:text-white">
      <h1 className="text-2xl font-bold mb-4">Settings</h1>

      <Tabs value={activeTab} onValueChange={setActiveTab} className="w-full">
        <div className="border-b border-gray-300 dark:border-gray-700 mb-4">
          <TabsList className="bg-white dark:bg-gray-800">
            <TabsTrigger value="general">General</TabsTrigger>
            <TabsTrigger value="display">Display</TabsTrigger>
            <TabsTrigger value="alerts">Alerts</TabsTrigger>
            <TabsTrigger value="system">System</TabsTrigger>
          </TabsList>
        </div>

        <TabsContent value="general">
          <div className="grid gap-6">
            <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
              <CardHeader>
                <CardTitle>User Settings</CardTitle>
                <CardDescription>Configure your user preferences</CardDescription>
              </CardHeader>
              <CardContent className="space-y-4">
                <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                  <div className="space-y-2">
                    <Label htmlFor="username">Username</Label>
                    <Input id="username" defaultValue="admin" />
                  </div>
                  <div className="space-y-2">
                    <Label htmlFor="email">Email</Label>
                    <Input id="email" type="email" defaultValue="admin@ubcformulaelectric.com" />
                  </div>
                </div>

                <div className="space-y-2">
                  <Label htmlFor="team">Team</Label>
                  <Select defaultValue="engineering">
                    <SelectTrigger id="team">
                      <SelectValue placeholder="Select team" />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="engineering">Engineering</SelectItem>
                      <SelectItem value="electrical">Electrical</SelectItem>
                      <SelectItem value="mechanical">Mechanical</SelectItem>
                      <SelectItem value="software">Software</SelectItem>
                    </SelectContent>
                  </Select>
                </div>

                <div className="flex items-center justify-between">
                  <Label htmlFor="notifications">Enable notifications</Label>
                  <Switch id="notifications" defaultChecked />
                </div>
              </CardContent>
            </Card>

            <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
              <CardHeader>
                <CardTitle>Data Collection</CardTitle>
                <CardDescription>Configure data collection settings</CardDescription>
              </CardHeader>
              <CardContent className="space-y-4">
                <div className="flex items-center justify-between">
                  <Label htmlFor="telemetry">Collect telemetry data</Label>
                  <Switch id="telemetry" defaultChecked />
                </div>

                <div className="flex items-center justify-between">
                  <Label htmlFor="analytics">Share anonymous analytics</Label>
                  <Switch id="analytics" defaultChecked />
                </div>

                <div className="space-y-2">
                  <Label htmlFor="frequency">Data collection frequency</Label>
                  <Select defaultValue="high">
                    <SelectTrigger id="frequency">
                      <SelectValue placeholder="Select frequency" />
                    </SelectTrigger>
                    <SelectContent>
                      <SelectItem value="low">Low (1 Hz)</SelectItem>
                      <SelectItem value="medium">Medium (10 Hz)</SelectItem>
                      <SelectItem value="high">High (100 Hz)</SelectItem>
                      <SelectItem value="ultra">Ultra (1000 Hz)</SelectItem>
                    </SelectContent>
                  </Select>
                </div>
              </CardContent>
            </Card>
          </div>
        </TabsContent>

        <TabsContent value="display">
          <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
            <CardHeader>
              <CardTitle>Display Settings</CardTitle>
              <CardDescription>Customize how data is displayed</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="space-y-2">
                <Label htmlFor="units">Unit System</Label>
                <Select defaultValue="metric">
                  <SelectTrigger id="units">
                    <SelectValue placeholder="Select unit system" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="metric">Metric (km/h, °C)</SelectItem>
                    <SelectItem value="imperial">Imperial (mph, °F)</SelectItem>
                  </SelectContent>
                </Select>
              </div>

              <div className="space-y-2">
                <Label htmlFor="chartStyle">Chart Style</Label>
                <Select defaultValue="line">
                  <SelectTrigger id="chartStyle">
                    <SelectValue placeholder="Select chart style" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="line">Line</SelectItem>
                    <SelectItem value="bar">Bar</SelectItem>
                    <SelectItem value="scatter">Scatter</SelectItem>
                  </SelectContent>
                </Select>
              </div>

              <div className="flex items-center justify-between">
                <Label htmlFor="darkMode">Dark Mode</Label>
                <Switch id="darkMode" defaultChecked />
              </div>

              <div className="flex items-center justify-between">
                <Label htmlFor="animations">Enable animations</Label>
                <Switch id="animations" defaultChecked />
              </div>
            </CardContent>
          </Card>
        </TabsContent>

        <TabsContent value="alerts">
          <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
            <CardHeader>
              <CardTitle>Alert Settings</CardTitle>
              <CardDescription>Configure system alerts and notifications</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="flex items-center justify-between">
                <Label htmlFor="criticalAlerts">Critical Alerts</Label>
                <Switch id="criticalAlerts" defaultChecked />
              </div>

              <div className="flex items-center justify-between">
                <Label htmlFor="warningAlerts">Warning Alerts</Label>
                <Switch id="warningAlerts" defaultChecked />
              </div>

              <div className="flex items-center justify-between">
                <Label htmlFor="infoAlerts">Informational Alerts</Label>
                <Switch id="infoAlerts" defaultChecked />
              </div>

              <div className="space-y-2">
                <Label htmlFor="alertSound">Alert Sound</Label>
                <Select defaultValue="chime">
                  <SelectTrigger id="alertSound">
                    <SelectValue placeholder="Select alert sound" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="none">None</SelectItem>
                    <SelectItem value="chime">Chime</SelectItem>
                    <SelectItem value="bell">Bell</SelectItem>
                    <SelectItem value="alarm">Alarm</SelectItem>
                  </SelectContent>
                </Select>
              </div>
            </CardContent>
          </Card>
        </TabsContent>

        <TabsContent value="system">
          <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
            <CardHeader>
              <CardTitle>System Settings</CardTitle>
              <CardDescription>Configure system-wide settings</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <div className="space-y-2">
                <Label htmlFor="language">Language</Label>
                <Select defaultValue="en">
                  <SelectTrigger id="language">
                    <SelectValue placeholder="Select language" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="en">English</SelectItem>
                    <SelectItem value="fr">French</SelectItem>
                    <SelectItem value="es">Spanish</SelectItem>
                    <SelectItem value="de">German</SelectItem>
                  </SelectContent>
                </Select>
              </div>

              <div className="space-y-2">
                <Label htmlFor="timezone">Timezone</Label>
                <Select defaultValue="utc-8">
                  <SelectTrigger id="timezone">
                    <SelectValue placeholder="Select timezone" />
                  </SelectTrigger>
                  <SelectContent>
                    <SelectItem value="utc-8">Pacific Time (UTC-8)</SelectItem>
                    <SelectItem value="utc-5">Eastern Time (UTC-5)</SelectItem>
                    <SelectItem value="utc+0">UTC</SelectItem>
                    <SelectItem value="utc+1">Central European Time (UTC+1)</SelectItem>
                  </SelectContent>
                </Select>
              </div>

              <div className="flex items-center justify-between">
                <Label htmlFor="autoUpdate">Automatic Updates</Label>
                <Switch id="autoUpdate" defaultChecked />
              </div>

              <div className="flex items-center justify-between">
                <Label htmlFor="debugMode">Debug Mode</Label>
                <Switch id="debugMode" />
              </div>
            </CardContent>
          </Card>
        </TabsContent>
      </Tabs>
    </div>
  )
}
