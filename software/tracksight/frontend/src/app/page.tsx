'use client';

import { useEffect, useState, React } from 'react';
import { io } from "socket.io-client";
import { message, Button, Divider, Layout, Switch } from 'antd';
const { Header, Content } = Layout;

import styles from './page.module.css';
import NavBar from './components/navbar.tsx';
import Graph from './components/graph.tsx';
import Dashboard from './components/dashboard.tsx';

const FLASK_URL = "http://localhost:5000"

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");
    const [socketInstance, setSocketInstance] = useState("");
    const [loading, setLoading] = useState(true);
    const [graphs, setGraphs] = useState([]);
    const [zoomData, setZoomData] = useState([]);
    // determines if all graphs are supposed to zoom together or not
    const [sync, setSync] = useState(false);
    const [messageApi, contextHolder] = message.useMessage();

    //add a new graph
    const addGraph = () => {
        const newGraphId = Date.now();  
        setGraphs(prevGraphs => [...prevGraphs, newGraphId]);
    };

    //delete a graph
    const deleteGraph = (graphId) => {
        setGraphs(prevGraphs => prevGraphs.filter(id => id !== graphId));
    };

    //set sync for all graphs
    const setSyncAll = (sync) => {
        setSync(!sync);
    }

    useEffect(() => {
        // NOTE -> mac users may need to turn airplay reciever off in order to connect to the server
        const socket = io(FLASK_URL, {
            transports: ["websocket"],
            cors: {
                origin: "http://localhost:3000/",
            },
        });

        setSocketInstance(socket)
        socket.on("connect", (data) => {
            console.log(data);
            setLoading(false);
        });


        socket.on("disconnect", (data) => {
            console.log(data);
        });
        return () => {
            socket.disconnect();
        };
    }, []);

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
                    key={graphId} id={graphId} url={FLASK_URL} sync={sync} setZoomData={setZoomData} zoomData={zoomData}
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
