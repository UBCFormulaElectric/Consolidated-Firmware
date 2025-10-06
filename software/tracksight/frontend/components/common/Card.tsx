// TODO(evan): Add the ability for more items to be added to the header

type CardProps = {
    title: string;
    children?: React.ReactNode | React.ReactNode[] | string;
};

const Card: React.FC<CardProps> = (props) => {
    const { title, children } = props;

    return (
        <div className="flex flex-col bg-primary-100 border-2 border-primary-300 shadow-gray-300 shadow-md">
            <div className="font-semibold text-2xl py-5 px-8 border-b-2 border-inherit">
                {title}
            </div>
            <div className="px-8 py-5">
                {children}
            </div>
        </div>
    )
}

export default Card;