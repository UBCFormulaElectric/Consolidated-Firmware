'use client';
import { useEffect, useState, React } from 'react';
import { io } from "socket.io-client";
import { Layout } from 'antd';
const { Header, Content } = Layout;

import styles from './page.module.css';
import Dashboard from './components/dashboard';
import NavBar from './components/navbar';
import Visualize from './components/visualize';

const FLASK_URL = "http://localhost:5000"

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");
    const [socketInstance, setSocketInstance] = useState("");
    const [loading, setLoading] = useState(true);
    const [graphs, setGraphs] = useState([]);
    const [liveGraphs, setLiveGraphs] = useState([]);


    //add a new graph
    const addGraph = (live) => {
        const newGraphId = Date.now();  
        if (live) {
            setLiveGraphs(prevGraphs => [...prevGraphs, newGraphId]);
        } else {
            setGraphs(prevGraphs => [...prevGraphs, newGraphId]);
        }
    };


    //delete a graph
    const deleteGraph = (graphId, live) => {
        if (live) {
            setLiveGraphs(prevGraphs => prevGraphs.filter(id => id !== graphId));
        } else {
            setGraphs(prevGraphs => prevGraphs.filter(id => id !== graphId));
        }
    };

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
                {!loading && (
                    componentToDisplay === "visualize" ? (
                        <Visualize
                            addGraph={addGraph}
                            graphs={graphs}
                            liveGraphs={liveGraphs}
                            deleteGraph={deleteGraph}
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
