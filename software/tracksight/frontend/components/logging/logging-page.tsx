"use client"

import { useState } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { Button } from "@/components/ui/button"
import { Calendar } from "@/components/ui/calendar"
import { SaveIcon, Download, FileText, Trash2 } from "lucide-react"

export default function LoggingPage() {
  const [date, setDate] = useState<Date | undefined>(new Date())

  return (
    <div className="p-6 text-gray-800 dark:text-white">
      <h1 className="text-2xl font-bold mb-4">Logging</h1>

      <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
        <div className="lg:col-span-1">
          <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
            <CardHeader>
              <CardTitle className="flex items-center">
                <SaveIcon className="mr-2 h-5 w-5" />
                SD Card Status
              </CardTitle>
            </CardHeader>
            <CardContent>
              <div className="space-y-4">
                <div className="flex items-center justify-between">
                  <span>Status:</span>
                  <span className="text-green-500 font-medium">Connected</span>
                </div>
                <div className="flex items-center justify-between">
                  <span>Capacity:</span>
                  <span>32 GB</span>
                </div>
                <div className="flex items-center justify-between">
                  <span>Used Space:</span>
                  <span>12.4 GB (38%)</span>
                </div>
                <div className="flex items-center justify-between">
                  <span>Free Space:</span>
                  <span>19.6 GB (62%)</span>
                </div>
                <div className="flex items-center justify-between">
                  <span>File System:</span>
                  <span>FAT32</span>
                </div>

                <div className="h-2 bg-gray-300 dark:bg-gray-700 rounded-full overflow-hidden">
                  <div className="h-full bg-blue-500 rounded-full" style={{ width: "38%" }}></div>
                </div>

                <div className="pt-4 flex justify-between">
                  <Button variant="outline" size="sm" className="flex items-center">
                    <Download className="mr-1 h-4 w-4" />
                    Export All
                  </Button>
                  <Button variant="destructive" size="sm" className="flex items-center">
                    <Trash2 className="mr-1 h-4 w-4" />
                    Format Card
                  </Button>
                </div>
              </div>
            </CardContent>
          </Card>

          <div className="mt-6">
            <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
              <CardHeader>
                <CardTitle>Select Date</CardTitle>
              </CardHeader>
              <CardContent>
                <Calendar
                  mode="single"
                  selected={date}
                  onSelect={setDate}
                  className="rounded-md border border-gray-300 dark:border-gray-700"
                />
              </CardContent>
            </Card>
          </div>
        </div>

        <div className="lg:col-span-2">
          <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
            <CardHeader>
              <CardTitle className="flex items-center">
                <FileText className="mr-2 h-5 w-5" />
                Log Files
              </CardTitle>
            </CardHeader>
            <CardContent>
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <div className="text-sm text-gray-500 dark:text-gray-400">
                    Showing logs for {date?.toLocaleDateString() || "today"}
                  </div>
                  <Button variant="outline" size="sm">
                    Refresh
                  </Button>
                </div>

                <div className="border border-gray-300 dark:border-gray-700 rounded-md overflow-hidden">
                  <table className="w-full text-sm">
                    <thead className="bg-gray-100 dark:bg-gray-900">
                      <tr>
                        <th className="text-left py-2 px-4">Filename</th>
                        <th className="text-left py-2 px-4">Size</th>
                        <th className="text-left py-2 px-4">Created</th>
                        <th className="text-left py-2 px-4">Actions</th>
                      </tr>
                    </thead>
                    <tbody>
                      {[1, 2, 3, 4, 5].map((i) => (
                        <tr key={i} className="border-t border-gray-300 dark:border-gray-700">
                          <td className="py-2 px-4">
                            log_session_{i}_{date?.toISOString().split("T")[0]}.dat
                          </td>
                          <td className="py-2 px-4">{(Math.random() * 10 + 1).toFixed(2)} MB</td>
                          <td className="py-2 px-4">
                            {new Date(date?.setHours(9 + i) || Date.now()).toLocaleTimeString()}
                          </td>
                          <td className="py-2 px-4">
                            <div className="flex space-x-2">
                              <Button variant="ghost" size="sm" className="h-8 w-8 p-0">
                                <Download className="h-4 w-4" />
                              </Button>
                              <Button variant="ghost" size="sm" className="h-8 w-8 p-0">
                                <FileText className="h-4 w-4" />
                              </Button>
                              <Button variant="ghost" size="sm" className="h-8 w-8 p-0 text-red-500 hover:text-red-600">
                                <Trash2 className="h-4 w-4" />
                              </Button>
                            </div>
                          </td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </div>

                <div className="pt-4">
                  <Card className="bg-gray-100 dark:bg-gray-900 border-gray-300 dark:border-gray-700">
                    <CardHeader className="pb-2">
                      <CardTitle className="text-sm">Log Preview</CardTitle>
                    </CardHeader>
                    <CardContent>
                      <pre className="text-xs overflow-auto h-40">
                        {`[2023-05-17 09:15:23] INFO: System startup
[2023-05-17 09:15:24] INFO: Battery voltage: 380.2V
[2023-05-17 09:15:25] INFO: Motor temperature: 25.3°C
[2023-05-17 09:15:26] INFO: Vehicle state: STANDBY
[2023-05-17 09:15:30] INFO: User authentication successful
[2023-05-17 09:15:35] INFO: Vehicle state: READY
[2023-05-17 09:15:40] INFO: Throttle position: 0.15
[2023-05-17 09:15:41] WARNING: Battery cell 4 voltage deviation detected
[2023-05-17 09:15:45] INFO: Vehicle speed: 15.2 km/h
[2023-05-17 09:15:50] INFO: Motor RPM: 1250`}
                      </pre>
                    </CardContent>
                  </Card>
                </div>
              </div>
            </CardContent>
          </Card>
        </div>
      </div>
    </div>
  )
}
