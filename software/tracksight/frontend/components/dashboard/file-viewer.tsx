"use client"

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { Button } from "@/components/ui/button"

type File = {
  id: string
  name: string
  date: string
}

export default function FileViewer() {
  const files: File[] = [
    { id: "file1", name: "system_log_1", date: "Jan 5, 10:00:00 10:20:00" },
    { id: "file2", name: "system_log_2", date: "Jan 5, 10:20:00 10:40:00" },
    { id: "file3", name: "system_log_3", date: "Jan 5, 10:40:00 11:00:00" },
  ]

  return (
    <Card className="bg-white dark:bg-gray-800 border-gray-300 dark:border-gray-700">
      <CardHeader className="pb-2">
        <div className="flex items-center">
          <CardTitle className="text-lg font-medium">Files</CardTitle>
        </div>
      </CardHeader>
      <CardContent>
        <div className="space-y-2">
          <div className="grid grid-cols-2 gap-2 text-sm text-gray-400 px-2">
            <div>File Name</div>
            <div>Date/Time</div>
          </div>

          {files.map((file) => (
            <div
              key={file.id}
              className="grid grid-cols-2 gap-2 bg-gray-200 dark:bg-gray-700 rounded-md p-2 text-sm text-gray-800 dark:text-white"
            >
              <div>{file.name}</div>
              <div>{file.date}</div>
            </div>
          ))}

          <div className="mt-4 pt-4 border-t border-gray-300 dark:border-gray-700">
            <div className="grid grid-cols-2 gap-4">
              <div>
                <h4 className="text-sm font-medium mb-2">File Name</h4>
                <div className="bg-gray-200 dark:bg-gray-700 rounded-md p-2 text-sm text-gray-800 dark:text-white">
                  system_log_1
                </div>
              </div>
              <div>
                <h4 className="text-sm font-medium mb-2">Characteristics</h4>
                <div className="bg-gray-200 dark:bg-gray-700 rounded-md p-2 text-sm text-gray-800 dark:text-white">
                  warning: file not converted read
                </div>
              </div>
            </div>

            <div className="flex justify-end space-x-2 mt-4">
              <Button variant="outline" size="sm" className="text-xs">
                Upload
              </Button>
              <Button variant="outline" size="sm" className="text-xs">
                Clear All
              </Button>
            </div>
          </div>
        </div>
      </CardContent>
    </Card>
  )
}
