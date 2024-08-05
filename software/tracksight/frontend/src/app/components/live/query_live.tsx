"use client";
import { Dispatch, useEffect, useState } from "react";
import { Space, Switch } from "antd";

import DropdownMenu from "../db/dropdown_menu";
import { MessageInstance } from "antd/es/message/interface";

const UPDATE_INTERVAL_MS = 1000; // how often the graph updates

export interface QueryLiveProps {
  url: string;
  setData: Dispatch<{
    [name: string]: { times: Array<string>; values: Array<number> };
  }>;
  messageApi: MessageInstance;
}

const QueryLive = (props: QueryLiveProps) => {
  const [signals, setSignals] = useState<string[]>([]);
  const [allSignal, setAllSignal] = useState<string[]>([]);
  const [useLive, setUseLive] = useState<boolean>(false);

  const changeLive = (checked: boolean) => {
    setUseLive(checked);
  };

  useEffect(() => {
    fetch(props.url + "/signals/live", {
      method: "get",
    })
      .then((response) => response.json())
      .then((data) => setAllSignal(data))
      .catch((error) => console.log(error));
  }, []);

  useEffect(() => {
    if (useLive && signals.length > 0) {
      const interval = setInterval(() => {
        const newParams = new URLSearchParams({
          signals: signals.join(","),
        });

        fetch(props.url + "/query/live?" + newParams)
          .then((response) => {
            if (!response.ok) {
              return response.json().then((json) => {
                throw new Error(json["error"]);
              });
            } else {
              return response.json();
            }
          })
          .then((data) => props.setData(data))
          .catch((error) =>
            props.messageApi.open({ type: "error", content: error.toString() })
          );
      }, UPDATE_INTERVAL_MS);

      return () => {
        clearInterval(interval);
      };
    }
  }, [useLive, signals]);

  return (
    <div className="flex flex-col">
      <Space direction={"vertical"} size={"small"}>
        <p>Turn live signal on/off</p>
        <Switch onChange={changeLive} checked={useLive} />
        <DropdownMenu
          setOption={setSignals}
          selectedOptions={signals}
          options={allSignal}
          single={false}
          name={"Signal"}
        />
      </Space>
    </div>
  );
};

export default QueryLive;
