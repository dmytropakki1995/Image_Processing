import React from 'react'
import { State } from 'router5'
import { routeNames} from './Routes'
import UploadImage from '../components/ContentComponents/UploadImage'
import About from '../components/ContentComponents/About'
import { Spin } from 'antd'
export interface RootParams {
    route : State;
}
const Root: React.FC<RootParams> = ({route}) => {
    if (!route) {
       return <UploadImage/>;
    }
    switch (route.name) {
        case (routeNames.Upload): {
            return <UploadImage/>;
        }
        case (routeNames.About): {
            return <About/>;
        }
        default :
            return (<div> Page not Found </div>)
    }
}

export default Root
