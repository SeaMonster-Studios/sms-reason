import React from "react";

export class ReactErrorBoundary extends React.Component {
  constructor(props) {
    super(props);
    this.state = { error: false, errorInfo: null };
  }

  componentDidCatch(error, errorInfo) {
    this.setState({ error, errorInfo });
    if (this.props.onError) {
      this.props.onError(error, errorInfo);
    }
  }

  render() {
    if (this.state.error) {
      return this.props.renderFallback
        ? this.props.renderFallback(this.state.error, this.state.errorInfo)
        : null;
    } else {
      return this.props.children;
    }
  }
}
