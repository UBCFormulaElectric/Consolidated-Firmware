import Link from "next/link";

function Navbar() {
  return (
    <nav className="fixed top-0 left-0 z-50 h-min w-screen">
      <div className="flex flex-row items-center gap-6 select-none px-8 py-4">
        <Link href="/live">Live Data</Link>
        <Link href="/historical">Historical Data</Link>
      </div>
    </nav>
  );
}

export default Navbar;
