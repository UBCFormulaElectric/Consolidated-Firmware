// TODO(evan): Add the ability for more items to be added to the header

type CardProps = {
  title: string;
  children?: React.ReactNode | React.ReactNode[] | string;
};

const Card: React.FC<CardProps> = (props) => {
  const { title, children } = props;

  return (
    <div className="bg-primary-100 border-primary-300 flex flex-col border-2 shadow-md shadow-gray-300">
      <div className="border-b-2 border-inherit px-8 py-5 text-2xl font-semibold">{title}</div>
      <div className="px-8 py-5">{children}</div>
    </div>
  );
};

export default Card;
