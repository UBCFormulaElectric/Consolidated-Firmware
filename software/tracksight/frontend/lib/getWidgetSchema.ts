import { WIDGET_SCHEMAS } from "./widgetSchemas"

const WIDGET_SCHEMA_MAP = Object.fromEntries(
    WIDGET_SCHEMAS.map((schema) => [schema.type, schema]),
)

const getWidgetSchema = (widgetType: string) => WIDGET_SCHEMA_MAP[widgetType];

export default getWidgetSchema;
