"use client";
import React, { useState } from "react";
import { message, Layout } from "antd";
const { Header, Content } = Layout;
import NavBar from "./components/navbar";
import Dashboard from "./components/dashboard";
import Visualize from "./components/visualize";

const FLASK_URL = "http://localhost:5000";

const Home = () => {
  const [componentToDisplay, setComponentToDisplay] =
    useState<string>("visualize");
  const [graphs, setGraphs] = useState<number[]>([]);
  const [liveGraphs, setLiveGraphs] = useState<number[]>([]);
  const [messageApi, contextHolder] = message.useMessage();
  const [theme, setTheme] = useState<boolean>(true);

  //add a new graph
  const addGraph = (live: boolean) => {
    const newGraphId = Date.now();
    if (live) {
      setLiveGraphs((prevGraphs) => [...prevGraphs, newGraphId]);
    } else {
      setGraphs((prevGraphs) => [...prevGraphs, newGraphId]);
    }
  };

  //delete a graph
  const deleteGraph = (graphId: number, live: boolean) => {
    if (live) {
      setLiveGraphs((prevGraphs) => prevGraphs.filter((id) => id !== graphId));
    } else {
      setGraphs((prevGraphs) => prevGraphs.filter((id) => id !== graphId));
    }
  };

  return (
    <Layout
      className={`flex flex-col justify-between p-0 m-0 min-h-screen ${
        theme ? "text-white bg-black" : "text-black bg-white"
      }`}
    >
      <Header className="p-0">
        <NavBar
          updateFunction={setComponentToDisplay}
          theme={theme}
          setTheme={setTheme}
        />
      </Header>
      <Content>
        {contextHolder}
        {componentToDisplay === "visualize" ? (
          <Visualize
            addGraph={addGraph}
            graphs={graphs}
            liveGraphs={liveGraphs}
            deleteGraph={deleteGraph}
            url={FLASK_URL}
            messageApi={messageApi}
          />
        ) : (
          <Dashboard deleteGraph={deleteGraph} />
        )}
      </Content>
    </Layout>
  );
};

export default Home;
