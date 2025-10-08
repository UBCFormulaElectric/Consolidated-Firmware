import { Calendar } from "lucide-react"
import EditModeToggle from "./EditModeToggle";

const Navbar = () => (
  <nav className="fixed top-0 left-0 h-min z-50 border-primary-300 bg-primary-100 flex flex-col w-screen">
    <div className="flex flex-row justify-between items-center select-none border-b-2 border-inherit px-10 py-5">
      {/** TODO(evan): Make this dynamic and a selector **/}
      <div className="flex gap-4 items-center hover:cursor-pointer hover:text-primary-900 transition-colors px-2 py-1 rounded-md">
        <Calendar className="stroke-current" size={24} />
        <div className="flex flex-col items-start">
          <span className="text font-semibold">
            September 28th 2025
          </span>
          <span className="opacity-50 text-sm">
            Session 1
          </span>
        </div>
      </div>

      {/** TODO(evan): Add pause / play button here */ }

      <EditModeToggle />
    </div>

    {/** TODO(evan): Add the universal timeline here */}
  </nav>
);

export default Navbar;
