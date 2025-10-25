import type { FC, FocusEvent, HTMLAttributes } from 'react';

type EditableTextProps = {
  onChange: (text: string) => void;
  placeholder?: string;
  initialText: string;
} & HTMLAttributes<HTMLInputElement>;

const EditableText: FC<EditableTextProps> = (props) => {
  const {
    onChange,
    initialText = "",
    placeholder = "Click to edit",
    ...rest
  } = props;

  const handleBlur = (event: FocusEvent<HTMLInputElement>) => {
    onChange(event.target.value);
  };

  return (
    <input
      {...rest}
      suppressContentEditableWarning
      onBlur={handleBlur}
      placeholder={placeholder}
      defaultValue={initialText}
      className="focus:outline-none w-full duration-200"
    />
  );
}

export default EditableText;

