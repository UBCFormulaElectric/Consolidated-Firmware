'use client';
import { useEffect, useState, React } from 'react';
import { message, Button, Divider, Layout, Switch } from 'antd';
const { Header, Content } = Layout;

import { io } from "socket.io-client";
import styles from './page.module.css';
import NavBar from './components/navbar';
import Dashboard from './components/dashboard';
import { PlotRelayoutEvent } from 'plotly.js';
import Visualize from './components/visualize';

const FLASK_URL = "http://evanyl.pythonanywhere.com";
//const FLASK_URL = "http://localhost:3000";
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

    return (
        <Layout className={styles.main}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay} />
            </Header>
            <Content>
            {contextHolder}
                {!loading && (
                    componentToDisplay === "visualize" ? (
                        <Visualize
                            setSync={setSync}
                            addGraph={addGraph}
                            graphs={graphs}
                            sync={sync}
                            setZoomData={setZoomData}
                            deleteGraph={deleteGraph}
                            zoomData={zoomData}
                            url={FLASK_URL}
                            socket={socketInstance}
                        />
                    ) : (
                        <Dashboard />
                    )
                )}
            </Content>
        </Layout>
    );
}

export default Home;
