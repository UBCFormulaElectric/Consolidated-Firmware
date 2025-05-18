"use client"
import { Home, FileText, Database, Settings, SaveIcon, ChevronLeft, ChevronRight } from "lucide-react"
import {
  Sidebar as SidebarComponent,
  SidebarContent,
  SidebarFooter,
  SidebarHeader,
  SidebarMenu,
  SidebarMenuItem,
  SidebarMenuButton,
  useSidebar
} from "@/components/ui/sidebar"
import { ThemeToggle } from "@/components/theme-toggle"
import Image from "next/image"
import { useRouter } from "next/navigation"
import { useEffect, useState } from "react"

interface SidebarProps {
  activePage: string
  setActivePage: (page: string) => void
}

export default function Sidebar({ activePage, setActivePage }: SidebarProps) {
  const router = useRouter()
  const { state, toggleSidebar } = useSidebar()
  const isCollapsed = state === "collapsed"
  const [storedSidebarState, setStoredSidebarState] = useState<boolean | null>(null)
  
  // Load sidebar state from localStorage when component mounts
  useEffect(() => {
    if (typeof window !== 'undefined') {
      const storedState = localStorage.getItem('sidebar:state')
      const savedIsOpen = storedState === 'true'
      setStoredSidebarState(savedIsOpen)
      
      // If current sidebar state doesn't match stored state, update it
      if (savedIsOpen === isCollapsed) {
        toggleSidebar()
      }
    }
  }, [])
  
  // Update localStorage when sidebar state changes
  useEffect(() => {
    if (storedSidebarState !== null && typeof window !== 'undefined') {
      localStorage.setItem('sidebar:state', (!isCollapsed).toString())
    }
  }, [isCollapsed, storedSidebarState])
  
  // Handle toggle with state persistence
  const handleToggle = () => {
    toggleSidebar()
  }

  // Handle navigation to different pages
  const navigateTo = (page: string) => {
    setActivePage(page)
    
    switch(page) {
      case "dashboard":
        router.push("/")
        break
      case "user-guide":
        router.push("/user-guide")
        break
      case "live-data":
        router.push("/live-data")
        break
      case "logging":
        router.push("/logging")
        break
      case "settings":
        router.push("/settings")
        break
      default:
        router.push("/")
    }
  }
  
  return (
    <>
      {/* Floating bubble for toggles - positioned outside sidebar */}
      <div className="fixed bottom-6 left-[12px] z-50 flex flex-col items-center gap-3 bg-white dark:bg-gray-800 rounded-full py-3 px-3 shadow-lg border border-gray-200 dark:border-gray-700">
        <ThemeToggle />
        <div className="h-px w-6 bg-gray-200 dark:bg-gray-700 my-1"></div>
        <button 
          onClick={handleToggle}
          className="p-2 rounded-full hover:bg-gray-100 dark:hover:bg-gray-700 text-gray-800 dark:text-gray-200"
          aria-label={isCollapsed ? "Expand sidebar" : "Collapse sidebar"}
        >
          {isCollapsed ? (
            <ChevronRight className="h-4 w-4" />
          ) : (
            <ChevronLeft className="h-4 w-4" />
          )}
        </button>
      </div>
      
      <SidebarComponent collapsible="icon">
        <SidebarHeader className="flex py-4">
          <div className={`flex w-full ${isCollapsed ? 'justify-center' : 'justify-start pl-3'}`}>
            <Image 
              src="/logo.jpeg" 
              alt="Tracksight Logo"
              width={36}
              height={36}
              className="rounded-md"
              onClick={() => navigateTo("dashboard")}
              style={{ cursor: 'pointer' }}
            />
          </div>
        </SidebarHeader>
        <SidebarContent>
          <SidebarMenu>
            <SidebarMenuItem>
              <SidebarMenuButton
                isActive={activePage === "dashboard"}
                onClick={() => navigateTo("dashboard")}
                tooltip="Dashboard"
              >
                <Home className="size-5" />
                {!isCollapsed && <span className="ml-3">Dashboard</span>}
              </SidebarMenuButton>
            </SidebarMenuItem>
            <SidebarMenuItem>
              <SidebarMenuButton
                isActive={activePage === "user-guide"}
                onClick={() => navigateTo("user-guide")}
                tooltip="User Guide"
              >
                <FileText className="size-5" />
                {!isCollapsed && <span className="ml-3">User Guide</span>}
              </SidebarMenuButton>
            </SidebarMenuItem>
            <SidebarMenuItem>
              <SidebarMenuButton
                isActive={activePage === "live-data"}
                onClick={() => navigateTo("live-data")}
                tooltip="Live Data"
              >
                <Database className="size-5" />
                {!isCollapsed && <span className="ml-3">Live Data</span>}
              </SidebarMenuButton>
            </SidebarMenuItem>
            <SidebarMenuItem>
              <SidebarMenuButton
                isActive={activePage === "logging"}
                onClick={() => navigateTo("logging")}
                tooltip="Logging"
              >
                <SaveIcon className="size-5" />
                {!isCollapsed && <span className="ml-3">Logging</span>}
              </SidebarMenuButton>
            </SidebarMenuItem>
            <SidebarMenuItem>
              <SidebarMenuButton
                isActive={activePage === "settings"}
                onClick={() => navigateTo("settings")}
                tooltip="Settings"
              >
                <Settings className="size-5" />
                {!isCollapsed && <span>Settings</span>}
              </SidebarMenuButton>
            </SidebarMenuItem>
          </SidebarMenu>
        </SidebarContent>
        <SidebarFooter className="p-2">
          {/* Empty footer - controls moved to floating bubble */}
        </SidebarFooter>
      </SidebarComponent>
    </>
  )
}