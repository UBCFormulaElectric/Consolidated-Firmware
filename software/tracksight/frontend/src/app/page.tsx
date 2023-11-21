'use client';
import React, { useEffect, useState } from 'react';
import { message, Layout } from 'antd';
const { Header, Content } = Layout;
import { io, Socket } from "socket.io-client";
import styles from './page.module.css';
import NavBar from './components/navbar';
import Dashboard from './components/dashboard';
import EditSignals from './components/editSignals';
import Visualize from './components/visualize';

//const FLASK_URL = "http://evanyl.pythonanywhere.com";
const FLASK_URL = "http://localhost:5000";
const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState<string>('visualize');
    const [socketInstance, setSocketInstance] = useState<Socket | null>(null);
    const [loading, setLoading] = useState<boolean>(true);
    const [graphs, setGraphs] = useState<number[]>([]);
    const [liveGraphs, setLiveGraphs] = useState<number[]>([]);
    const [messageApi, contextHolder] = message.useMessage();
    const [theme, setTheme] = useState<boolean>(false);

    //add a new graph
    const addGraph = (live: boolean) => {
        const newGraphId = Date.now();
        if (live) {
            setLiveGraphs(prevGraphs => [...prevGraphs, newGraphId]);
        } else {
            setGraphs(prevGraphs => [...prevGraphs, newGraphId]);
        }
    };


    //delete a graph
    const deleteGraph = (graphId: number, live: boolean) => {
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
            transportOptions: {
                origin: "http://localhost:3000/",
            },
        });

        setSocketInstance(socket)

        socket.on("connect", () => {
            setLoading(false);
        });


        socket.on("disconnect", () => {
            setLoading(true);
        });
        return () => {
            socket.disconnect();
        };
    }, []);

    return (
        <Layout className={`${styles.main} ${theme ? styles.dark : styles.light}`}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay} theme={theme} setTheme={setTheme} />
            </Header>
            <Content>
                {contextHolder}
                {!loading && (
                    componentToDisplay === "visualize" ? (
                        <Visualize
                            addGraph={addGraph}
                            graphs={graphs}
                            liveGraphs={liveGraphs}
                            deleteGraph={deleteGraph}
                            url={FLASK_URL}
                            socket={socketInstance!}
                            messageApi={messageApi}
                        />
                    ) : (
                        componentToDisplay === "editSignals" ? (
                            <EditSignals />
                        ) : (
                            <Dashboard />
                        )
                    )
                )}
            </Content>
        </Layout>
    );
}

export default Home;
