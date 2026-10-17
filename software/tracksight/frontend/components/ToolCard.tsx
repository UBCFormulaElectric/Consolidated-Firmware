import { ExternalLink, Play } from "lucide-react";

type DAMToolCardProps = {
    title: string;
    description: string;
    type: "page" | "button";
    onClick?: () => void;
    href?: string;
    icon: React.ReactNode;
};

function DAMToolCard({ title, description, href, icon, type, onClick }: DAMToolCardProps) {
    const content = (
        <>
            <div className="flex flex-row justify-between">
                <span className="text-lg font-semibold flex items-center gap-2">
                    {icon}
                    {title}
                </span>
                <span className="text-lg font-semibold">{type === "button" ? <Play className="size-5 group-hover:text-green-500 transition-colors" /> : <ExternalLink className="size-5 transition-colors group-hover:text-blue-500" />}</span>
            </div>

            <span className="text-sm text-gray-500">{description}</span>
        </>
    );

    return type === "button" ? (
        <button className="flex flex-col group gap-2 hover:bg-gray-500/5 transition-colors border-black border rounded p-4 text-left cursor-pointer" onClick={onClick}>
            {content}
        </button>
    ) : (
        <a className="flex flex-col group gap-2 border-black hover:bg-black/5 transition-colors border rounded p-4" href={href}>
            {content}
        </a>
    );
}

export default DAMToolCard;
