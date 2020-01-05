import createRouter from 'router5';
import browserPlugin from 'router5-plugin-browser';
import persistentParamsPlugin from 'router5-plugin-persistent-params'
import {routes, routeNames} from './Routes'

const router = createRouter(routes);
router.usePlugin(
    browserPlugin({
        useHash: false
    })
)
router.start(routeNames.Upload);

export default router
