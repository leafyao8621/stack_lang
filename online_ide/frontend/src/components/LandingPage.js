import React, { useEffect, useState, useRef } from "react";
import axios from "axios";
import { Link, useLocation } from "react-router-dom";
import { Input } from "reactstrap"

const LandingPage = (props) => {
    const location  = useLocation();
    const [files, setFiles] = useState();
    const [examples, setExamples] = useState();
    const [fileName, setFileName] = useState();
    const loaded = useRef();
    useEffect(() => {
        if (!loaded.current) {
            axios.get(
                "/api/files/list_files/",
                {
                    params: {
                        user_name: location.state.userName
                    }
                }
            ).then((res) => {
                if (res.data.success) {
                    setFiles(res.data.data.map((item) => item.file_name));
                } else {
                    alert("Error loading files");
                }
            }).catch((err) => {
                alert("Error loading files");
                console.log(err);
            });
            axios.get(
                "/api/examples/list_examples/",
                {
                    params: {
                        user_name: location.state.userName
                    }
                }
            ).then((res) => {
                if (res.data.success) {
                    setExamples(res.data.data.map((item) => item.file_name));
                } else {
                    alert("Error loading examples");
                }
            }).catch((err) => {
                alert("Error loading examples");
                console.log(err);
            });
            loaded.current = true;
        }
    });
    const renderFiles = () => {
        if (!files) {
            return "";
        }
        return (
            <li
                className="list-group-item"
            >
                Files
                <ul
                    className="list-group"
                >
                    {
                        files.map((item) => {
                            return (
                                <li
                                    className="list-group-item"
                                    key={item}
                                >
                                    <Link
                                        to="/editor"
                                        state={{
                                            userName: location.state.userName,
                                            fileName: item,
                                            example: false
                                        }}
                                    >
                                        {item}
                                    </Link>
                                </li>
                            )
                        })
                    }
                </ul>
            </li>
        );
    }
    const renderExamples = () => {
        if (!examples) {
            return "";
        }
        return (
            <li
                className="list-group-item"
            >
                Examples
                <ul
                    className="list-group"
                >
                    {
                        examples.map((item) => {
                            return (
                                <li
                                    className="list-group-item"
                                    key={item}
                                >
                                    <Link
                                        to="/editor"
                                        state={{
                                            userName: location.state.userName,
                                            fileName: item,
                                            example: true
                                        }}
                                    >
                                        {item}
                                    </Link>
                                </li>
                            )
                        })
                    }
                </ul>
            </li>
        );
    }
    return (
        <main className="container">
            <h1>Logged in as { location.state.userName }</h1>
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
                                to={{
                                    pathname: "/"
                                }}
                            >
                                Logout
                            </Link>
                        </li>
                        <li
                            className="list-group-item"
                        >
                            <Input
                                type="text"
                                id="un"
                                placeholder="Enter a file name"
                                onChange={({ target }) => {
                                    setFileName(target.value);
                                }}
                            />
                            <Link
                                to="/editor"
                                state={{
                                    userName: location.state.userName,
                                    fileName: fileName,
                                    example: false,
                                    newFile: true
                                }}
                            >
                                Create
                            </Link>
                        </li>
                    </ul>
                </li>
                { renderFiles() }
                { renderExamples() }
            </ul>
        </main>
    );
};

export default LandingPage;
