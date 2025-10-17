import { Calendar } from "lucide-react";

import EditModeToggle from "@/components/EditModeToggle";

const Navbar = () => (
  <nav className="border-primary-300 bg-primary-100 fixed top-0 left-0 z-50 flex h-min w-screen flex-col">
    <div className="flex flex-row items-center justify-between border-b-2 border-inherit px-10 py-5 select-none">
      {/** TODO(evan): Make this dynamic and a selector **/}
      <div className="hover:text-primary-900 flex items-center gap-4 rounded-md px-2 py-1 transition-colors hover:cursor-pointer">
        <Calendar className="stroke-current" size={24} />
        <div className="flex flex-col items-start">
          <span className="text font-semibold">September 28th 2025</span>
          <span className="text-sm opacity-50">Session 1</span>
        </div>
      </div>

      {/** TODO(evan): Add pause / play button here */}

      <EditModeToggle />
    </div>

    {/** TODO(evan): Add the universal timeline here */}
  </nav>
);

export default Navbar;
