import React, { useEffect, useState } from 'react';
import { getRealtimeData, deleteDashboardData } from '../../../dashboardService';
import { CheckboxChangeEvent } from 'antd/lib/checkbox';
import { Select, Space, Button, Modal, Checkbox, List, message } from 'antd';
import { Socket } from "socket.io-client";
import LiveGraph from './live/livegraph';

// currently a bug where loading dashboards only loads SOME of the signals on graphs (seems indeterministic)
// found error occurs in livegraphs where it checks if data['id'] == graphId, and thus does not update graph,
// as id sometimes returns as 0 when returned from websocket process. unsure how to fix. 


export interface DashboardProps {
  socket: Socket; 
  deleteGraph: (graphId: number, live: boolean) => void;
}
const Dashboard = ( props: DashboardProps ) => {
    const [items, setItems] = useState<Array<{value: string, label: string}>>([]);
    const [selectedDashboard, setSelectedDashboard] = useState("");
    const [toDelete, setToDelete] = useState<string[]>([]);
    const [graphs, setGraphs] = useState({});
    const [modalOpen, setModalOpen] = useState<boolean>(false);

    const updateGraphSignals = (graphId: number, signals: string[]) => {
        setGraphs(prev => ({ ...prev, [graphId]: signals }));
    };

    // change handler for selecting dashboard to load from dropdown 
    const handleChange = (value: string) => {
      setSelectedDashboard(value);
    };

    // delete graph from dashboard
    const deleteGraph = (graphId: number) => {
      setGraphs(prevGraphs => {
        const newGraphs = { ...prevGraphs };
        delete newGraphs[graphId as keyof typeof newGraphs];
        return newGraphs;
      });
    };

    // get the list of available saved dashboards from firestore
  useEffect(() => {
    getRealtimeData("dashboards").then(data => {
      let d = [];
      for (const [key, value] of Object.entries(data)) {
        d.push({value: key, label: key});
      }
      setItems(d);
    }
    )
  }, []);

  // load the selected dashboard from firestore
    const loadDashboard = () => {
      setGraphs({})
        getRealtimeData("dashboards").then(data => {
          if (data && selectedDashboard in data) {
            setGraphs(data[selectedDashboard].graphs);
        } else {
          message.error("Failed to load/find dashboard.");
        }
        });

    };

    // updates toDelete array accordingly when checkbox is checked/unchecked
    const handleCheckboxChange = (e: CheckboxChangeEvent, value: string) => {
      setToDelete(prev => {
        if (e.target.checked) {
          return [...prev, value];
        } else {
          return prev.filter(item => item !== value);
        }
      });
    };

  // deletes selected dashboards from firestore
  const deleteDashboards = () => {
    if (toDelete.length === 0) {
        message.warning('No dashboards selected for deletion.');
        return;
    }

    Modal.confirm({
        title: 'Are you sure you want to delete the selected dashboards?',
        content: 'This action cannot be undone.',
        okText: 'Yes, Delete',
        cancelText: 'No, Cancel',
        onOk: async () => {
            for (const dashboardName of toDelete) {
                const dashboardPath = `dashboards/${dashboardName}`;
                await deleteDashboardData(dashboardPath);
            }
            message.success("Selected dashboards deleted");
            setToDelete([]);
            setModalOpen(false);
        },
    });
};


    return (
        <div className="layout">
          <Space size="middle">
           <Select
            showSearch
            style={{minWidth: "200px", maxWidth: "100%"}}
            placeholder={"Select Dashboard to Load"}
            onChange={handleChange}
            options={items}
            maxTagCount="responsive"
        />
        <Button onClick={loadDashboard}>Load</Button>
        <Button onClick={() => setModalOpen(true)}>Edit Saved Dashboards</Button>
        <Modal open={modalOpen} 
        closeIcon={false} title='Delete Saved Dashboards'
                    cancelButtonProps={{onClick:() =>setModalOpen(false)}}
                    okButtonProps={{onClick:()=>deleteDashboards()}}
                    okText={"Delete Selected"}>
                        <List
                            size="small"
                            bordered
                            dataSource={items}
                            renderItem={item => (
                                <List.Item>
                                  <Checkbox 
                                    onChange={(e) => handleCheckboxChange(e, item.value)}
                                    checked={toDelete.includes(item.value)}>
                                    {item.label}
                                    </Checkbox>
                                </List.Item>
                            )}
                        />
                    </Modal>
        </Space>
            <div className="graph-container">
            {Object.entries(graphs).map(([graphId, signals]) => (
        <LiveGraph
          key={graphId}
          id={parseInt(graphId)} 
          onDelete={() => deleteGraph(parseInt(graphId))}
          updateGraphSignals={updateGraphSignals}
          socket={props.socket}
          signals={signals as string[]}
        />
    ))}
            </div>
            
        </div>
    );
};

export default Dashboard;