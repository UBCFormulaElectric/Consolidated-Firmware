import ToolsPageContent from "@/components/ToolsPageContent";

export default async function ToolsPage() {
    return (
        <div className="px-14 gap-8 h-screen flex flex-col pb-20 overflow-hidden">
            <div className="pt-20 w-screen" />
            <span className="text-3xl font-bold">Tools</span>
            <span className="text-sm text-gray-500">Tools for managing and interacting with the car.</span>

            <ToolsPageContent />
        </div>
    );
}
