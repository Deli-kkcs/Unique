using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExcludeField : MonoBehaviour
{
    [SerializeField][Header("ÅÅ³âÁ¦: Õý-ÅÅ³â ¸º-ÎüÒý")]
    private float excludeForce = 1000f;
    [SerializeField][Header("ÅÅ³â°ë¾¶")]
    private float excludeRadius = 5f;

    private void Update()
    {
        GetComponent<SphereCollider>().radius = excludeRadius;
    }

    private void OnTriggerStay(Collider other)
    {
        if(GetComponent<SphereCollider>().enabled == false)
        {
            return;
        }
        if (other.tag == "Cube")
        {
            Vector3 dir = other.transform.position - transform.position;
            other.GetComponent<Rigidbody>().AddForce(dir.normalized * excludeForce);
        }
    }
    void TestOverlap()
    {
        Collider[] colliders = Physics.OverlapSphere(transform.position, excludeRadius);
    }
}
