'use client';

import {  useState } from 'react';
import { message, Button, Divider, Layout, Switch } from 'antd';
const { Header, Content } = Layout;

import styles from './page.module.css';
import NavBar from './components/navbar';
import Graph from './components/graph';
import Dashboard from './components/dashboard';
import { PlotRelayoutEvent } from 'plotly.js';

const FLASK_URL = "https://evanyl.pythonanywhere.com";
//const FLASK_URL = "http://localhost:3000";

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");
    //TODO: When we need to load stuff reimplement this
    //const [loading, setLoading] = useState(true);
    const [loading, setLoading] = useState(false);
    const [graphs, setGraphs] = useState<Array<string>>([]);
    const [zoomData, setZoomData] = useState<PlotRelayoutEvent>({});
    // determines if all graphs are supposed to zoom together or not
    const [sync, setSync] = useState(false);
    const [messageApi, contextHolder] = message.useMessage();

    //add a new graph
    const addGraph = () => {
        const newGraphId = Date.now().toString();  
        setGraphs(prevGraphs => [...prevGraphs, newGraphId]);
    };

    //delete a graph
    const deleteGraph = (graphId: string) => {
        setGraphs(prevGraphs => prevGraphs.filter(id => id !== graphId));
    };

    //set sync for all graphs
    const setSyncAll = (sync: boolean) => {
        setSync(!sync);
    }

    let componentToRender;
    if (componentToDisplay === "visualize") {
        componentToRender = (
        <div className="layout">
            <h1>Visualize</h1>
            <p>Select a signal from the dropdown menu and the press submit to visualize the data on the graph.</p>
            <div>
                 <Switch onClick={setSync}></Switch>
                    <p>Sync Zoom</p>
            </div>
            <Button onClick={addGraph}>Add Another Graph</Button>
            <Divider></Divider>
            <div className="flex-container">
            {graphs.map(graphId => (
                 <Graph 
                    graphid={graphId} id={graphId} url={FLASK_URL} sync={sync} setZoomData={setZoomData} zoomData={zoomData}
                    onDelete={() => deleteGraph(graphId)} messageApi={messageApi}
                 />
            ))}
            </div>
        </div>);
    } else {
        componentToRender = (<Dashboard></Dashboard>);
    }

    return (
        <Layout className={styles.main}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay}/>
            </Header>
            <Content>
                {contextHolder}
                {!loading && componentToRender}
            </Content>
        </Layout>
    );
}

export default Home;
