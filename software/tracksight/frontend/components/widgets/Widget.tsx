"use client";

import { useEditMode } from "@/lib/contexts/EditModeContext";
import getWidgetSchema from "@/lib/getWidgetSchema";
import WidgetRenderer, { WIDGET_TYPE } from "@/lib/types/Widget";

const Widget: WidgetRenderer<WIDGET_TYPE> = (props) => {
  const { type } = props;

  const { isEditMode } = useEditMode();

  const widgetSchema = getWidgetSchema(type);

  const Renderer = isEditMode ? widgetSchema.editor : widgetSchema.renderer;

  if (!Renderer) {
    console.error(`Attempted to render widget with non-existant type '${type}'`);

    return;
  }

  return <Renderer {...props} />;
};

export default Widget;
