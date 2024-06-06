'use client';
import React, { useEffect, useState } from 'react';
import { message, Layout } from 'antd';
const { Header, Content } = Layout;
import styles from './page.module.css';
import NavBar from './components/navbar';
import Dashboard from './components/dashboard';
import Visualize from './components/visualize';

const FLASK_URL = "http://localhost:5000";

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState<string>('visualize');
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

    return (
        <Layout className={`${styles.main} ${theme ? styles.dark : styles.light}`}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay} theme={theme} setTheme={setTheme} />
            </Header>
            <Content>
                {contextHolder}
                {
                    componentToDisplay === "visualize" ? (
                        <Visualize
                            addGraph={addGraph}
                            graphs={graphs}
                            liveGraphs={liveGraphs}
                            deleteGraph={deleteGraph}
                            url={FLASK_URL}
                            messageApi={messageApi}
                        />
                    ) : (
                        <Dashboard
                            socket={null!}
                            deleteGraph={deleteGraph}

                        />
                    )
                }
            </Content>
        </Layout>
    );
}

export default Home;