type EnumValueEditorProps = {
  enumOptions: string[];
  colorPalette: string[];
}

const EnumValueEditor: React.FC<EnumValueEditorProps> = (props) => {
  const {
    enumOptions,
    colorPalette
  } = props;

  return (
    <div className="flex flex-row gap-4 flex-wrap">
      <p className="text-lg font-medium ">
        Color Palette:
      </p>
      {
        enumOptions.length > 0
          ? enumOptions.map((option, index) => (
            <div key={option.toString()} className="flex flex-row gap-1 items-center">
              <div className="w-6 h-6 rounded border border-gray-300" style={{ backgroundColor: colorPalette[index] || "#000" }}></div>
              {option.toString()}
            </div>
          ))
          : (
            <div className="text-gray-500 italic">Select a signal to see its enum options.</div>
          )
      }
    </div>
  )
}

export default EnumValueEditor;
