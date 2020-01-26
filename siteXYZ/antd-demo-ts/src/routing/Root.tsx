import React from 'react'
import { State } from 'router5'
import { routeNames} from './Routes'
import UploadImage from '../components/ContentComponents/UploadImage'
import About from '../components/ContentComponents/About'
import { Spin } from 'antd'
import Faq from '../components/ContentComponents/Faq'
import Donation from '../components/ContentComponents/Donation'
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
        case (routeNames.Faq): {
            return <Faq/>;
        }
        case (routeNames.Donation): {
            return <Donation/>;
        }
        default :
            return (<div> Page not Found </div>)
    }
}

export default Root
