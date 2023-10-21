'use client';

import { useEffect, useState, React } from 'react';
import { io } from "socket.io-client";
import { Layout, Divider, Button } from 'antd';
const { Header, Content } = Layout;

import styles from './page.module.css';
import NavBar from './components/navbar.tsx';
import Graph from './components/graph.tsx';
import Dashboard from './components/dashboard.tsx';
import DropdownMenu from './components/dropdown_menu';

const FLASK_URL = "http://localhost:5000"

const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");
    const [socketInstance, setSocketInstance] = useState("");
    const [loading, setLoading] = useState(true);
    const [graphs, setGraphs] = useState([]);

    // logic to add a new graph
    const addGraph = () => {
        const newGraphId = Date.now();  
        setGraphs(prevGraphs => [...prevGraphs, newGraphId]);
    };

    const deleteGraph = (graphId) => {
        setGraphs(prevGraphs => prevGraphs.filter(id => id !== graphId));
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

    let componentToRender;
    if (componentToDisplay === "visualize") {
        componentToRender = (
        <div className="layout">
            <h1>Visualize</h1>
            <p>Select a signal from the dropdown menu and the press submit to visualize the data on the graph.</p>
            <Button onClick={addGraph}>Add Another Graph</Button>
            <Divider></Divider>
            <div className="flex-container">
            {graphs.map(graphId => (
                 <Graph 
                    key={graphId} id={graphId} url={FLASK_URL}
                    onDelete={() => deleteGraph(graphId)}
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
                {!loading && componentToRender}
            </Content>
        </Layout>
    );
}

export default Home;
