import SDCardDumpPage from "../../../components/sdCard/SDCardDumpPage";

export default function HomePage() {
  return (
    <div
      className="px-14 gap-8 min-h-screen flex flex-col pb-20 overflow-hidden"
      style={{ height: "100vh" }}
    >
      <div className="pt-20 w-screen" />
      <span className="text-3xl font-bold">Dump SD Card</span>
      <span className="text-sm text-gray-500">
        Select an SD card and files to dump their contents to the database.
      </span>

      <SDCardDumpPage />
    </div>
  );
}
