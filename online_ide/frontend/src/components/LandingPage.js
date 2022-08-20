import React, { useEffect, useState } from "react";
import axios from "axios";
import { Link, useLocation } from "react-router-dom";

const LandingPage = (props) => {
    const location  = useLocation();
    const [files, setFiles] = useState();
    const [examples, setExamples] = useState();
    useEffect(() => {
        axios.get(
            "/api/files/list_files/",
            {
                params: {
                    user_name: location.state.userName
                }
            }
        ).then((res) => {
            setFiles(res.data.data.map((item) => item.file_name));
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
            setExamples(res.data.data.map((item) => item.file_name));
        }).catch((err) => {
            alert("Error loading examples");
            console.log(err);
        });
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
                                        to={{
                                            pathname: "/"
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
                                        to={{
                                            pathname: "/"
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
                    <Link
                        to={{
                            pathname: "/"
                        }}
                    >
                        Logout
                    </Link>
                </li>
                { renderFiles() }
                { renderExamples() }
            </ul>
        </main>
    );
}

export default LandingPage;
