import React, { useEffect, useState, useRef } from "react";
import axios from "axios";
import { useLocation, Link } from "react-router-dom";
import CodeMirror from '@uiw/react-codemirror';
import { Button } from "reactstrap";
import { w3cwebsocket } from 'websocket';

const Editor = (props) => {
    const location = useLocation();
    const [content, setContent] = useState();
    const [input, setInput] = useState();
    const [output, setOutput] = useState();
    const ws = useRef();
    const loaded = useRef();
    useEffect(() => {
        if (!loaded.current && !location.state.newFile) {
            if (!location.state.example) {
                axios.get(
                    "/api/files/retrieve_file/",
                    {
                        params: {
                            user_name: location.state.userName,
                            file_name: location.state.fileName
                        }
                    }
                ).then((res) => {
                    if (res.data.success) {
                        setContent(res.data.data);
                        setInput(res.data.input);
                    } else {
                        alert("Error loading file");
                    }
                }).catch((err) => {
                    alert("Error loading file");
                    console.log(err);
                });
            } else {
                axios.get(
                    "/api/examples/retrieve_example/",
                    {
                        params: {
                            file_name: location.state.fileName
                        }
                    }
                ).then((res) => {
                    if (res.data.success) {
                        setContent(res.data.data);
                        setInput(res.data.input);
                    } else {
                        alert("Error loading file");
                    }
                }).catch((err) => {
                    alert("Error loading file");
                    console.log(err);
                });
            }
            loaded.current = true;
        }
        if (location.state.newFile && !loaded.current) {
            setInput("");
            setContent("");
            loaded.current = true;
        }
        ws.current =
            w3cwebsocket(
                `ws://localhost:8000/ws/worker/${location.state.userName}/`
            );
        ws.current.onmessage = (msg) => {
            const data = JSON.parse(msg.data);
            if (data.success) {
                alert("success");
            } else {
                alert("failure");
            }
            setOutput(`${data.message}\n${data.output}`)
        }
        return () => ws.current.close();
    },
    [location.state]);
    const onChangeContent = React.useCallback((value, viewUpdate) => {
        setContent(value);
    }, []);
    const onChangeInput = React.useCallback((value, viewUpdate) => {
        setInput(value);
    }, []);
    const onSave = () => {
        axios.post(
            "/api/files/update_file/",
            {
                user_name: location.state.userName,
                file_name: location.state.fileName,
                data: content,
                input: input
            }
        ).then((res) => {
            if (res.data.success) {
                alert("success");
            } else {
                alert("failure");
            }
        }).catch((err) => {
            alert("failure");
        })
    };
    const onCompile = () => {
        ws.current.send(JSON.stringify({
            user_name: location.state.userName,
            file_name: location.state.fileName,
            action: "compile"
        }));
    };
    const onRun = () => {
        if (location.state.example) {
            ws.current.send(JSON.stringify({
                user_name: location.state.userName,
                file_name: location.state.fileName,
                action: "run example"
            }));
        } else {
            ws.current.send(JSON.stringify({
                user_name: location.state.userName,
                file_name: location.state.fileName,
                action: "run"
            }));
        }
    };
    return (
        <main className="container">
            <h1>Editing {location.state.fileName} </h1>
            <ul
                className="list-group"
            >
                <li
                    className="list-group-item"
                >
                    <ul
                        className="list-group"
                    >
                        <li
                            className="list-group-item"
                        >
                            <Link
                                to="/landing"
                                state={{
                                    userName: location.state.userName
                                }}
                            >
                                Back
                            </Link>
                        </li>
                        {
                            !location.state.example ?
                            <li
                                className="list-group-item"
                            >
                                <Button
                                    onClick={onSave}
                                >
                                    Save
                                </Button>
                            </li> :
                            ""
                        }
                        {
                            !location.state.example ?
                            <li
                                className="list-group-item"
                            >
                                <Button
                                    onClick={onCompile}
                                >
                                    Compile
                                </Button>
                            </li> :
                            ""
                        }
                        <li
                            className="list-group-item"
                        >
                            <Button
                                onClick={onRun}
                            >
                                Run
                            </Button>
                        </li>
                    </ul>
                </li>
                <li
                    className="list-group-item"
                >
                    Code
                    <CodeMirror
                        value={content}
                        height="500px"
                        onChange={onChangeContent}
                    />
                </li>
                <li
                    className="list-group-item"
                >
                    Output
                    <CodeMirror
                        value={output}
                        height="200px"
                    />
                </li>
                <li
                    className="list-group-item"
                >
                    Input
                    <CodeMirror
                        value={input}
                        height="200px"
                        onChange={onChangeInput}
                    />
                </li>
            </ul>
        </main>
    );
};

export default Editor;
