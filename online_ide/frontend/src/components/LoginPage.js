import React, { Component } from "react";
import { Navigate, useLocation } from "react-router-dom";
import axios from "axios";

import {
    Button,
    Form,
    FormGroup,
    Input,
    Label,
} from "reactstrap";

class LoginPage extends Component {
    constructor(props) {
        super(props);
        this.state = {
            userName: "",
            loggedIn: false
        };
    }

    render() {
        const login = (e) => {
            e.preventDefault();
            axios.post(
                "/api/users/login/",
                {
                    user_name: this.state.userName
                }
            ).then((res) => {
                this.setState({loggedIn: true});
            }).catch((err) => {
                alert("Failed");
            });

        }
        if (this.state.loggedIn) {
            return (
                <Navigate
                    to="/landing"
                    state={ {userName: this.state.userName} }
                />
            )
        }
        return (
            <main className="container">
                <h1>Stack Lang</h1>
                <Form onSubmit={login}>
                    <FormGroup>
                        <Label for="un">User Name</Label>
                        <Input
                            type="text"
                            id="un"
                            placeholder="Enter a user name"
                            onChange={({ target }) => {
                                this.setState({userName: target.value}
                            )}}
                        />
                    </FormGroup>
                    <Button type="submit">Login</Button>
                </Form>
            </main>
        );
    }
}

export default LoginPage;
