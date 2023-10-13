'use client';

import { useEffect, useState, React } from 'react';
import { io } from "socket.io-client";
import { Layout } from 'antd';
const { Header, Content } = Layout;

import styles from './page.module.css';
import NavBar from './components/navbar.tsx';
import Graph from './components/graph.tsx';
import DropdownMenu from './components/dropdown_menu';

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");
    const [socketInstance, setSocketInstance] = useState("");
    const [loading, setLoading] = useState(true);

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

        setLoading(false);

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
        <div>
            <DropdownMenu socket={socketInstance} />
            <Graph socket={socketInstance}/>
        </div>);
    } else {
        componentToRender = (<p>Hello</p>);
    }

    return (
        <Layout className={styles.main}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay}/>
            </Header>
            <Content>
                {!loading && componentToRender}
            </Content>
        </Layout>
    );
}

export default Home;
