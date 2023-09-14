'use client';

import {useState, React} from 'react';
import styles from './page.module.css'
import NavBar from './components/navbar.tsx'

import { Layout } from 'antd';
const { Header, Content } = Layout;


const Home = () => {
    const [componentToDisplay, setComponentToDisplay] = useState("visualize");

    let componentToRender = null;
    if (componentToDisplay === "visualize") {
        componentToRender = (<p>Hi</p>);
    } else {
        componentToRender = (<p>Hello</p>);
    }

    return (
        <Layout className={styles.main}>
            <Header className={styles.header}>
                <NavBar updateFunction={setComponentToDisplay}/>
            </Header>
            <Content>
                {componentToRender}
            </Content>
        </Layout>
    )
}

export default Home;
