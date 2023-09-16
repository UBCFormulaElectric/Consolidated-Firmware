'use client';

import { useEffect, useState, React } from 'react';
import { io } from "socket.io-client";
import { Layout } from 'antd';
const { Header, Content } = Layout;

import styles from './page.module.css';
import NavBar from './components/navbar.tsx';
import Graph from './components/graph.tsx';

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");
    const [socketInstance, setSocketInstance] = useState("");

    useEffect(() => {
        const socket = io("localhost:5000/", {
            transports: ["websocket"],
            cors: {
                origin: "http://localhost:3000/",
            },
        });

        setSocketInstance(socket)
        socket.on("connect", (data) => {
            console.log(data);
        });

        socket.on("disconnect", (data) => {
            console.log(data);
        });
    });

    let componentToRender = null;
    useEffect(() => {
        if (componentToDisplay === "visualize") {
            componentToRender = (<Graph socket={socketInstance}/>);
        } else {
            componentToRender = (<p>Hello</p>);
        }
    }, [componentToDisplay]);

    return (
        <Layout className={styles.main}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay}/>
            </Header>
            <Content>
                {componentToRender}
            </Content>
        </Layout>
    );
}

export default Home;
