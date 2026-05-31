import { LucideIcon } from "lucide-react";

type PanelEmptyStateProps = {
  Icon: LucideIcon;
  title: string;
  description?: string;
};

const PanelEmptyState = (props: PanelEmptyStateProps) => {
  const { Icon, title, description } = props;

  return (
    <div className="h-full flex flex-col items-center gap-4 justify-center">
      <Icon className="size-12 text-gray-300 mt-20" />
      <div className="flex flex-col items-center gap-2">
        <span className="text-gray-500 max-w-48 text-center text-lg">{title}</span>
        {description && (
          <span className="text-gray-500 max-w-48 text-center opacity-75">{description}</span>
        )}
      </div>
    </div>
  );
};

export default PanelEmptyState;
