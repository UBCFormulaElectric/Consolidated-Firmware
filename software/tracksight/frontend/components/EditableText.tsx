import React from 'react';

type EditableText = {
  onChange: (text: string) => void;
  placeholder?: string;
  initialText: string;
};

const EditableText: React.FC<EditableText> = (props) => {
  const {
    onChange,
    initialText = "",
    placeholder = "Click to edit",
  } = props;

  const handleBlur = (event: React.FocusEvent<HTMLInputElement>) => {
    onChange(event.target.value);
  };

  return (
    <input
      suppressContentEditableWarning
      onBlur={handleBlur}
      placeholder={placeholder}
      defaultValue={initialText}
      className="focus:outline-none w-full duration-200"
    />
  );
}

export default EditableText;

